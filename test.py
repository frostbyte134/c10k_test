import requests
import smtplib
from email.mime.text import MIMEText
import json
import time

def send_gmail(title, body):
    #587포트 및 465포트 존재
    smtp = smtplib.SMTP('smtp.gmail.com', 587)

    smtp.ehlo()

    smtp.starttls()

    #로그인을 통해 지메일 접속
    smtp.login('nailbrainz@gmail.com', 'plcsjusdonizxdve')

    #내용을 입력하는 MIMEText => 다른 라이브러리 사용 가능
    msg = MIMEText(body)
    msg['Subject'] = title

    #이메일을 보내기 위한 설정(Cc도 가능)
    smtp.sendmail('nailbrainz@gmail.com', ['nailbrainz@gmail.com', "otsegoletric@naver.com"], msg.as_string())

    #객체 닫기
    smtp.quit()



while True:
    try:

        data = requests.get("https://soui.kr/product/classic-shirt-over-dyed-check-sky-bluebrown/2648/category/112/display/1/#none",
                            headers={"User-Agent": "Mozilla/5.0"})
        resp_str = data.content.decode('utf-8')

        for i, aline in enumerate(resp_str.split("\n")):
            # print(i, aline)

            if "var option_stock_data =" in aline:
                aline_trimed = aline[:aline.find("var stock_manage")]

                json_str = aline_trimed[aline_trimed.find("{"):aline_trimed.rfind("}") + 1].replace("\\", "")
                print(json_str)

                stock_data = json.loads(json_str)
                print(stock_data)

                print()
                if stock_data["P0000DXW000A"]["stock_number"] == 0:
                    print('sending email!')
                    send_gmail("mailbag black chuistock on", "gogogo")
                    print(' email sent!')

    except Exception as e:
        send_gmail("error on bot", str(e))

    time.sleep(600)
