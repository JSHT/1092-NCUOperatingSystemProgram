import requests
import jsonpath
from fake_useragent import UserAgent
import threading
import os

def song_download(name, i, url, title, author, header, img, lrc):
    # 建立資料夾
    mDir = "./music/" + name
    lDir = "./lyric/" + name
    # iDir = "./image/" + name
    os.makedirs(mDir, exist_ok=True)
    os.makedirs(lDir, exist_ok=True)
    # os.makedirs(iDir, exist_ok=True)
    print('歌曲:{0}-{1},正在下載...'.format(title, author))
    # 下載（這種讀寫檔案的下載方式適合少量檔案的下載）
    try:
        content = requests.get(url, headers=header, timeout=10).content
        imgContent = requests.get(img, headers=header, timeout=3).content
    except TimeoutError:
        return
    file_name = "./music/" + name + "/" + title + "_" + author + ".mp3"
    lrcName = "./lyric/" + name + "/" + title + "_" + author + ".lrc"
    # imgName = "./image/" + name + "/" + title + "_" + author + ".jpg"

    with open(file_name, mode='wb') as f:
        f.write(content)
    if lrc != None:
        with open(lrcName, mode='w', encoding="utf-8") as f:
            f.write(lrc)
    # with open(imgName, mode='wb') as f:
    #     f.write(imgContent)
    print('下載完畢,{0}-{1},請試聽'.format(title, author))


def get_music_name(name, platform, pages):
    ua = UserAgent(verify_ssl=False)
    url = 'https://music.liuzhijin.cn/'
    headers = {
        "user-agent": ua.random,
        "x-requested-with": "XMLHttpRequest",
    }
    param = {
        "input": name,
        "filter": "name",
        "type": platform,
        "page": pages,
    }
    res = requests.post(url=url, data=param, headers=headers)
    json_text = res.json()
    # print(json_text)
    img = jsonpath.jsonpath(json_text, '$..pic')
    title = jsonpath.jsonpath(json_text, '$..title')
    author = jsonpath.jsonpath(json_text, '$..author')
    url = jsonpath.jsonpath(json_text, '$..url')
    lrc = jsonpath.jsonpath(json_text, '$..lrc')
    # print(lrc)
    if title:
        songs = list(zip(title, author, url, img, lrc))
        i = 1
        for s in songs:
            print(i, s[0], s[1])
            song_download(name, i, url[i - 1], title[i - 1],
                          author[i - 1], headers, img[i - 1], lrc[i-1])
            i += 1
        print("-------------------------------------------------------")
    else:
        print("對不起，暫無搜尋結果!")


if __name__ == "__main__":
    name = input("請輸入歌曲或歌手名稱:")
    num = int(input("請輸入要抓幾頁歌(一頁十首)(1~10):"))
    print(name)
    threads = []
    strings = ["kugou", "netease", "kuwo"]
    pages = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
    for i in range(1):
        for j in range(num):
            threads.append(threading.Thread(
                target=get_music_name, args=(name, strings[i], pages[j])))
            threads[num*i+j].start()
    for i in range(1):
        for j in range(num):
            threads[num*i+j].join()
    print("Done.")
