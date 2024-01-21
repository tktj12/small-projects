import cv2
import mediapipe as mp
import math
import time
import bluetooth
import RPi.GPIO as GPIO
import pigpio
import os
import subprocess
import threading
import webbrowser
import random

class LCD:
    def __init__(self):
        self.first_line = ' '
        self.second_line = ' '

    # 사용법 : lcd.set(윗줄 문자열 or None, 아랫줄 문자열 or None)
    # None을 주면 해당 줄 문자열이 유지됨.
    def set(self, msg1, msg2 = None):
        if msg1 is not None:
            self.first_line = msg1
        if msg2 is not None:
            self.second_line = msg2
        subprocess.Popen(['sudo' ,'-E', '-u', 'capi','/home/capi/lcd', self.first_line, self.second_line])

lcd = LCD()
lcd.set('Loading.....', 'Please Wait')

os.system('sudo hciconfig hci0 piscan')

class Sound:
    def __init__(self):
        self.volume = 90    # 볼륨 설정 기본값

    def play(self, arg):
        subprocess.Popen(['mplayer', '-ao', 'alsa:device=hw=UACDemoV10', '/home/capi/voices/'+arg+'.mp3'])

    def setVolume(self, v):
        self.volume = v
        self.volume = max(0, min(self.volume, 100))     # 0~100 사이로 제한
        subprocess.Popen(['amixer', 'sset', 'Master', str(self.volume) + '%'])

    def good(self):
        self.play('beep')

    def volumeUp(self, v):
        self.setVolume(self.volume + v)

    def volumeDown(self, v):
        self.setVolume(self.volume - v)

sound = Sound()

# GPIO 초기화
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)

# led gpio 설정
led_pin = 21
GPIO.setup(led_pin, GPIO.OUT)
GPIO.output(led_pin, False)

#--------------------------------------------------------------------------------------
#블루투스 소캣 통신
bluetooth_connection_flag = False

port = 1
server_socket = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
server_socket.bind(("", port))
server_socket.listen(1)

server_uuid = "1bab2098-bb0f-44d0-a170-b036349ba66c"
bluetooth.advertise_service(server_socket, "MyBluetoothServer", service_id=server_uuid)

global client_socket 
global client_address 
#---------------------------------------------------------------------------------------
global start_flag #start 버튼
global flag #운동 횟수를 올리기위한 flag
global cnt #운동 횟수
stop_flag = False
start_flag = False 
flag = 0 
cnt = 0 
detect = False #checkdetection 에 활용
#--------------------------------------------------------------------------------------
mp_pose = mp.solutions.pose
pose = mp_pose.Pose(static_image_mode=False, min_detection_confidence=0.5, min_tracking_confidence=0.5)

left_shoulder = mp_pose.PoseLandmark.LEFT_SHOULDER.value
left_elbow = mp_pose.PoseLandmark.LEFT_ELBOW.value
left_wrist = mp_pose.PoseLandmark.LEFT_WRIST.value
right_shoulder = mp_pose.PoseLandmark.RIGHT_SHOULDER.value
right_elbow = mp_pose.PoseLandmark.RIGHT_ELBOW.value
right_wrist = mp_pose.PoseLandmark.RIGHT_WRIST.value
left_hip = mp_pose.PoseLandmark.LEFT_HIP.value
left_knee = mp_pose.PoseLandmark.LEFT_KNEE.value
right_hip = mp_pose.PoseLandmark.RIGHT_HIP.value
right_knee = mp_pose.PoseLandmark.RIGHT_KNEE.value 
right_ankle = mp_pose.PoseLandmark.RIGHT_ANKLE.value
left_ankle = mp_pose.PoseLandmark.LEFT_ANKLE.value
#---------

def timer():
    global stop_flag, state
    s = time.time()
    while not bluetooth_connection_flag:
        e = time.time() - s
        if e > 5 :
            sound.play('bluetooth_disconnection')
            lcd.set('Bluetooth', 'Disconnected')
            stop_flag = True
            state = 'main'
            break

    while not bluetooth_connection_flag: pass

    if e > 5:
        sound.play('bluetooth_recovery')
        lcd.set('Choose exercise', 'in mypt app')

def connection():
    global client_socket 
    global client_address
    global bluetooth_connection_flag
    print("Waiting for connection...")
    bluetooth_connection_flag = False
    threading.Thread(target=timer).start()
    client_socket, client_address = server_socket.accept()
    bluetooth_connection_flag = True
    print("Connected to:", client_address) 

def LEDOnOff(_onoff):
    GPIO.output(led_pin, _onoff)

def mode(_index,_landmarks):
    if _index == 'pushup':
        pushup(_landmarks)
    elif _index == 'pullup':
        pullup(_landmarks)
    elif _index == 'squat':
        squat(_landmarks)

def receive_data():
    global start_flag, state, stop_flag, cnt
    print("receive_data on")
    slider_mode = ' '
    while True:
        try:
            received_data = client_socket.recv(1024)
        except bluetooth.btcommon.BluetoothError as e:
            connection()
            continue
        except KeyboardInterrupt:
            print("Key interrupt terminating program")
            return

        if len(received_data) > 0:
            data = received_data.decode()
            print("Received data:", data)

            if   data == 'main':
                lcd.set('Choose exercise', 'in mypt app')
                state = data
            elif data == 'select_page':
                lcd.set('Choose exercise', 'in mypt app')
                state = data
            elif data == 'set_page':
                state = data
            elif data == 'record_page':
                state = data
            elif data == 'pushup' : 
                motor.set(0)  # 푸시업
                state = data
            elif data == 'pullup' : 
                motor.set(45) # 풀업
                state = data
            elif data == 'squat' : 
                motor.set(20) # 스쿼트
                state = data
            if data == 'start':
                if state == 'pushup' or state == 'pullup' or state == 'squat':
                    sound.play('start_'+state)
                    lcd.set('mode : '+state, 'count : 0')
                    start_flag = True
            elif data == 'stop' :
                stop_flag = True  # stop_flag 설정하여 stream_video 스레드에 종료 신호 전달
            elif data == 'volume':
                slider_mode = data
            elif data == 'motor':
                slider_mode = data
            else:
                try:
                    i_data = int(data)
                except ValueError:
                    continue
                if i_data > 100 : continue
                if slider_mode == 'volume':
                    sound.setVolume(i_data)
                elif slider_mode == 'motor':
                    motor.set(i_data)

def main():
    global cnt
    global start_flag, state, flag, wrist_position, stop_flag, check_flag
    stop_flag = False
    check_flag = False

    start_time = time.time()

    print("stream_video on")
    video_capture = cv2.VideoCapture("/dev/video0")
    video_capture.set(cv2.CAP_PROP_FPS, 5)             # 프레임 속도 설정
    video_capture.set(cv2.CAP_PROP_FRAME_WIDTH, 640)    # 해상도 너비 설정
    video_capture.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)   # 해상도 높이 설정

    ret, frame = video_capture.read()
    frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    results = pose.process(frame_rgb)
    
    if state == 'pullup':
        wrist_position = 10.0
        
    flag = 1
    cnt = frame_count = 0   
    while not stop_flag:
        ret, frame = video_capture.read()

        if not ret:
            break
        frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

        if frame_count%2 == 0:
            results = pose.process(frame_rgb)
            if results.pose_landmarks:
                landmarks = results.pose_landmarks.landmark
                mode(state,landmarks)

                mp_drawing = mp.solutions.drawing_utils
                mp_drawing.draw_landmarks(frame, results.pose_landmarks, mp_pose.POSE_CONNECTIONS)
            else:
                LEDOnOff(False)
                if check_flag == True:
                    check_flag = False
                    os.system('sudo kill $(pidof -x mplayer)')
                    sound.play('no_recognize')
        frame_count += 1
        
        #cv2.imshow('Webcam', frame)
        cv2.waitKey(1)

    LEDOnOff(False)
    if state is not 'main':
        sound.play('end_' + state)
    video_capture.release()
    #cv2.destroyAllWindows()

    end_time = time.time()

    execution_time = end_time - start_time
    print("time: ",execution_time,"'s")
    try:
        send_data = str(int(execution_time))    # 운동시간 전송
        client_socket.send(send_data.encode())
    except:
        pass
    start_flag = False
    cv2.destroyAllWindows()

def check_detection(_arr,_landmarks):
    global check_flag
    checkcnt = 0
    for index in _arr:                  # index : 각 관절에 해당하는 인덱스
        landmark = _landmarks[index]    # 해당 관절에 대한 정보
        if landmark.visibility > 0.5:   # 인식이 50% 이상 됐을 때
            checkcnt +=1                

    # 필요한 모든 관절이 인식됐을 때 True를 리턴
    # 인식 여부에 따라 음성 출력
    if checkcnt == len(_arr):
        if check_flag == False:
            check_flag = True
            os.system('sudo kill $(pidof -x mplayer)')
            sound.play('recognize')
        return True
    else:
        if check_flag == True:
            check_flag = False
            os.system('sudo kill $(pidof -x mplayer)')
            sound.play('no_recognize')
        return False
    
def calculate_angle(a, b, c):
    ba = [a.x - b.x, a.y - b.y]
    bc = [c.x - b.x, c.y - b.y]

    dot_product = ba[0] * bc[0] + ba[1] * bc[1]

    ba_mag = math.sqrt(ba[0] * ba[0] + ba[1] * ba[1])
    bc_mag = math.sqrt(bc[0] * bc[0] + bc[1] * bc[1])
    angle_rad = math.acos(dot_product / (ba_mag * bc_mag))
    angle_deg = math.degrees(angle_rad)

    return angle_deg

def pushup(_landmarks):
    global stop_flag
    if state != 'pushup':
        stop_flag = True
        return

    if  check_detection({left_hip, left_shoulder,left_elbow,left_wrist},_landmarks) == True:
        check_tmp = True
        arr = [left_hip, left_shoulder,left_elbow,left_wrist]
    elif check_detection({right_hip, right_shoulder,right_elbow,right_wrist},_landmarks) == True:
        check_tmp = True
        arr = [right_hip, right_shoulder,right_elbow,right_wrist]
    else:
        check_tmp = False

    if check_tmp is True: 
        if _landmarks[arr[0]].y > _landmarks[arr[3]].y or _landmarks[arr[1]].y > _landmarks[arr[3]].y or _landmarks[arr[2]].y > _landmarks[arr[3]].y:
            check_tmp = False

    if check_tmp == True:      
        LEDOnOff(True)
        global flag
        global cnt

        arm_angle = calculate_angle(_landmarks[arr[1]], _landmarks[arr[2]], _landmarks[arr[3]])
        shoulder_angle = calculate_angle(_landmarks[arr[0]], _landmarks[arr[1]], _landmarks[arr[2]])
        
        if flag == 1 and arm_angle < 105 and shoulder_angle < 90:
            sound.good()
            flag = 0
            
        elif flag == 0 and arm_angle > 150 and shoulder_angle > 40:
            cnt +=1
            flag = 1
            print("push-up:", cnt)
            send_data = str(cnt)    # 개수 전송
            client_socket.send(send_data.encode())
            lcd.set(None, 'count : '+ str(cnt))
            sound.play(str(cnt))
    else:
        LEDOnOff(False)
        print("cant")        

def squat(_landmarks):
    global stop_flag
    if state != 'squat':
        stop_flag = True
        return
    
    check_tmp = check_detection({left_hip, left_knee, left_ankle, right_hip, right_knee, right_ankle},_landmarks)

    if check_tmp == True and _landmarks[left_hip].y > _landmarks[left_ankle].y:
        check_tmp = False

    if check_tmp == True:   
        LEDOnOff(True)
        global flag
        global cnt 
        left_angle = calculate_angle(_landmarks[left_hip], _landmarks[left_knee], _landmarks[left_ankle])
        right_angle = calculate_angle(_landmarks[right_hip], _landmarks[right_knee], _landmarks[right_ankle])

        print(left_angle, right_angle)
        if flag == 1 and left_angle < 105 and right_angle < 105:
            flag = 0
            sound.good()
            
        elif flag == 0 and left_angle > 150 and right_angle > 150 and _landmarks[left_hip].y < _landmarks[left_knee].y:
            cnt +=1
            print("squat:", cnt)
            send_data = str(cnt)    # 개수 전송
            client_socket.send(send_data.encode())
            lcd.set(None, 'count : '+ str(cnt))
            sound.play(str(cnt))
            flag = 1
    else:
        LEDOnOff(False)
        print("cant") 

def pullup(_landmarks):
    global stop_flag, flag, cnt, wrist_position

    if state != 'pullup':
        stop_flag = True
        return

    arr = {left_shoulder,left_elbow,left_wrist,right_shoulder,right_elbow,right_wrist}
    check_tmp = check_detection(arr,_landmarks)
    if check_tmp == True:      
        LEDOnOff(True)

        dif = _landmarks[left_elbow].y - _landmarks[left_wrist].y

        left_height = _landmarks[left_shoulder].y - _landmarks[left_wrist].y
        right_height = _landmarks[right_shoulder].y - _landmarks[right_wrist].y

        if flag == 1 and _landmarks[left_shoulder].y > _landmarks[left_elbow].y and _landmarks[right_shoulder].y > _landmarks[right_elbow].y and left_height > dif and right_height > dif and dif > 0:
            if abs(_landmarks[left_wrist].y - wrist_position) > dif/2:
                if wrist_position > 9.:
                    flag = 0
                    sound.good()
                return 
            flag = 0
            sound.good()
            
        #print(left_height, right_height, 'dif: ',dif)
        if flag == 0 and dif > 0 and left_height < dif and right_height < dif:
            if wrist_position > 9.:
                wrist_position = _landmarks[left_wrist].y
            elif abs(_landmarks[left_wrist].y - wrist_position) > dif/2:
                return
            flag = 1
            cnt +=1
            print("pull-up:", cnt)
                
            send_data = str(cnt)    # 개수 전송
            client_socket.send(send_data.encode())
            lcd.set(None, 'count : '+ str(cnt))
            sound.play(str(cnt))
    else:
        LEDOnOff(False)
        print("cant")

    #wrist_position = _landmarks[left_wrist].y # 야매모드

class Motor:
    def __init__(self, gpio_num):
        os.system('sudo pigpiod')                   # gpio 백그라운드 제어 daemon
        time.sleep(5)
        self.pin = gpio_num                         # 모터 제어 핀
        self.pi = pigpio.pi()                       # gpio 객체 생성
        self.pi.set_servo_pulsewidth(self.pin, 0)   # pwm 신호 초기화
        self.angle = 0                              # 멤버 변수 초기화

    def __del__(self):
        GPIO.cleanup()  # GPIO 해제

    # angle 변수에 각도 값을 바로 줌. 
    def set(self, angle):
        self.angle = angle
        duty_cycle = 1700 - self.angle*12
        self.pi.set_servo_pulsewidth(self.pin, duty_cycle)

    def up(self):
        self.set(self.angle+10)
    def down(self):
        self.set(self.angle-10)

motor = Motor(12)
motor.set(0)

# 최초 블루투스 연결
print("Waiting for connection...")
lcd.set('Need Bluetooth', 'Connection')
sound.play('bluetooth_connection')
client_socket, client_address = server_socket.accept()
print("Connected to:", client_address) 
sound.play('bluetooth_recovery')
lcd.set('Choose exercise', 'in mypt app')

threading.Thread(target=receive_data).start()
state = 'main'

while True:
    try:
        if start_flag == True: main()
    except KeyboardInterrupt:
        print("Key interrupt, terminating program")
        break

client_socket.close()
server_socket.close()