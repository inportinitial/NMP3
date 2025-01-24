import sys
import time
import subprocess
import asyncio
from bilibili_api import video, Credential, HEADERS
from PySide6.QtCore import QObject,Signal
from urllib.parse import urlparse,parse_qs
import httpx
import os

SESSDATA = ""
BILI_JCT = ""
BUVID3 = ""
DOWNLOAD_URL = ""
DOWNLOAD_TYPE = ""
DOWNLOAD_DIR = ""

FFMPEG_PATH = "ffmpeg"

class DownloadBiliBiliVideoDevice():
    def __init__(self,sessdata,bili_jct,buvid3):
        self.sessdata = sessdata
        self.bili_jct = bili_jct
        self.buvid3 = buvid3
        self.loop = asyncio.new_event_loop()
        self.count = 0
        asyncio.set_event_loop(self.loop)

    def get_video_name(self,url):
        credential = Credential(sessdata=self.sessdata, bili_jct=self.bili_jct, buvid3=self.buvid3)
        bvid = self.get_bv_and_p(url=url)[0]
        v = video.Video(bvid=bvid,credential=credential)
        info = self.loop.run_until_complete(v.get_info())
        list = '<>*|\\/:?"'
        name = info['title']
        ans = ""
        for i in range(0,len(name)):
            if name[i] in list:
                ans+='_'
            else:
                ans+=name[i]
        return ans

    def get_bv_and_p(self,url:str):
        bvid = 1
        p = 1
        processed_url = parse_qs(urlparse(url=url).query)
        if 'bvid' in processed_url:
            bvid = processed_url["bvid"][0]
        elif 'BV' in url:
            ans = ""
            find_bv = 0
            for c in url:
                if find_bv:
                    if c.isdigit() or c.islower() or c.isupper():
                        ans+=c
                    else:
                        break
                elif c == 'B':
                    ans+=c
                    find_bv = 1
            bvid = ans
        else:
            print("error bvid not in url")
            sys.stdout.flush()
        if 'p' in processed_url:
            p = int(processed_url["p"][0])
        p-=1
        return (bvid,p)

    def download_mp4(self,mp4_url):
        bvid,p = self.get_bv_and_p(mp4_url)
        self.loop.run_until_complete(self._download_mp4(bvid=bvid,p=p,output_file_name=self.get_video_name(mp4_url)))

    def download_mp3(self,mp3_url):
        bvid,p = self.get_bv_and_p(mp3_url)
        self.loop.run_until_complete(self._download_mp3(bvid=bvid,p=p,output_file_name=self.get_video_name(mp3_url)))

    async def download_url(self,url: str, out: str, info: str):
        # 下载函数
        async with httpx.AsyncClient(headers=HEADERS) as sess:
            resp = await sess.get(url)
            length = resp.headers.get('content-length')
            with open(out, 'wb') as f:
                process = 0
                for chunk in resp.iter_bytes(1024):
                    if not chunk:
                        break
    
                    process += len(chunk)
                    self.count+=1
                    if self.count == 10000:
                        time.sleep(0.1)
                        print(f'download_progress {process} / {length}')
                        sys.stdout.flush()
                        self.count = 0
                    f.write(chunk)

    async def _download_mp3(self,bvid,p,output_file_name):
        # 实例化 Credential 类
        credential = Credential(sessdata=self.sessdata, bili_jct=self.bili_jct, buvid3=self.buvid3)
        # 实例化 Video 类
        v = video.Video(bvid=bvid, credential=credential)
        # 获取视频下载链接
        download_url_data = await v.get_download_url(page_index=p)
        # 解析视频下载信息
        detecter = video.VideoDownloadURLDataDetecter(data=download_url_data)
        streams = detecter.detect_best_streams()
        # 有 MP4 流 / FLV 流两种可能
        time.sleep(0.1)
        print(f"begin_download {output_file_name}")
        sys.stdout.flush()
        if detecter.check_flv_stream() == True:
            time.sleep(0.1)
            print("warning not mp3 but flv")
            sys.stdout.flush()
        else:
            # MP4 流下载
            await self.download_url(streams[1].url, f"{output_file_name}_audio.m4s", "音频流")
            time.sleep(0.1)
            print("download_audio_done")
            sys.stdout.flush()
            subprocess.run(f'{FFMPEG_PATH} -y -i "{output_file_name}_audio.m4s" -vn -acodec libmp3lame -q:a 2 "{DOWNLOAD_DIR}\\{output_file_name}.mp3"')
            # 删除临时文件
            os.remove(f"{output_file_name}_audio.m4s")
        time.sleep(0.1)
        print("download_done")
        sys.stdout.flush()
        return

    async def _download_mp4(self,bvid,p,output_file_name):
        # 实例化 Credential 类
        credential = Credential(sessdata=self.sessdata, bili_jct=self.bili_jct, buvid3=self.buvid3)
        # 实例化 Video 类
        v = video.Video(bvid=bvid, credential=credential)
        # 获取视频下载链接
        download_url_data = await v.get_download_url(page_index=p)
        # 解析视频下载信息
        detecter = video.VideoDownloadURLDataDetecter(data=download_url_data)
        streams = detecter.detect_best_streams()
        time.sleep(0.1)
        print(f"begin_download {output_file_name}")
        sys.stdout.flush()
        # 有 MP4 流 / FLV 流两种可能
        if detecter.check_flv_stream() == True:
            time.sleep(0.1)
            print("warning download mp4 but flv")
            sys.stdout.flush()
            # FLV 流下载
            await self.download_url(streams[0].url, "flv_temp.flv", "FLV 音视频流")
            # 转换文件格式
            subprocess.run(f'{FFMPEG_PATH} -i flv_temp.flv video.mp4')
            # 删除临时文件
            os.remove("flv_temp.flv")
        else:
            # MP4 流下载
            await self.download_url(streams[0].url, f'{output_file_name}_video.m4s', "视频流")
            time.sleep(0.1)
            print("download_video_done")
            sys.stdout.flush()
            await self.download_url(streams[1].url, f'{output_file_name}_audio.m4s', "音频流")
            time.sleep(0.1)
            print("download_audio_done")
            sys.stdout.flush()
            # 混流
            subprocess.run(f'{FFMPEG_PATH} -y -i "{output_file_name}_video.m4s" -i "{output_file_name}_audio.m4s" -vcodec copy -acodec copy "{DOWNLOAD_DIR}\\{output_file_name}.mp4"')
            # 删除临时文件
            os.remove(f'{output_file_name}_video.m4s')
            os.remove(f'{output_file_name}_audio.m4s')
        time.sleep(0.1)
        print("download_done")
        sys.stdout.flush()
        return

valid_parameter_amount = 0

for i in range(1,len(sys.argv)):
    data = sys.argv[i]
    if data.startswith('-'):
        if data == "-sessdata" :
            SESSDATA = sys.argv[i+1]
            valid_parameter_amount += 1
        elif data == "-bili_jct":
            BILI_JCT = sys.argv[i+1]
            valid_parameter_amount += 1
        elif data == "-buvid3":
            BUVID3 = sys.argv[i+1]
            valid_parameter_amount += 1
        elif data == "-download_url":
            DOWNLOAD_URL = sys.argv[i+1]
            valid_parameter_amount += 1
        elif data == "-download_type":
            DOWNLOAD_TYPE = sys.argv[i+1]
            valid_parameter_amount += 1
        elif data == "-download_dir_path":
            DOWNLOAD_DIR = sys.argv[i+1]
            valid_parameter_amount += 1

if valid_parameter_amount == 6:
    device = DownloadBiliBiliVideoDevice(sessdata=SESSDATA,bili_jct=BILI_JCT,buvid3=BUVID3)
    DOWNLOAD_DIR.replace("/","\\")
    if 0:#for debug
        time.sleep(0.1)
        print(f"download_dir:{DOWNLOAD_DIR}")
        sys.stdout.flush()
    
    if DOWNLOAD_TYPE == "mp3":
        device.download_mp3(DOWNLOAD_URL)
    elif DOWNLOAD_TYPE == "mp4":
        device.download_mp4(DOWNLOAD_URL)
else:
    print("error input invalid")