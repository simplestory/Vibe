# coding = utf-8

import sys
sys.path.append("./lib")

import time
import cv2
from VIBE import VIBE


vidpath = "./data/vtest.avi"
mask_path = "./mask.avi"

cap = cv2.VideoCapture(vidpath)

# for saving video
vid_width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
vid_height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
vid_fps = int(cap.get(cv2.CAP_PROP_FPS))
fourcc = cv2.VideoWriter_fourcc(*'XVID')
maskvid = cv2.VideoWriter(mask_path, fourcc, vid_fps, (vid_width, vid_height), isColor=False)

# Initialize the VIBE class
bgvibe = VIBE()
bgvibe.sampleNums = 20  # 各像素模型数
bgvibe.miniMatch = 6  # 最小背景匹配数
bgvibe.radis = 20  # 背景模型最大偏差
bgvibe.rateNum = 16  # 背景更新概率种子(1/rateNum)

while True:
    ret, frame = cap.read()
    if not ret:
        break
    
    start = time.time()
    segmask = bgvibe.Vibe(frame)
    end = time.time()

    print("Update time: {:.2f}ms".format((end-start)*1000))
    maskvid.write(segmask)

cap.release()
maskvid.release()