
import gspread
from oauth2client.service_account import ServiceAccountCredentials
import os
import sys
PATH = os.path.abspath(os.path.dirname(sys.argv[0]))
scope = ["https://www.googleapis.com/auth/spreadsheets", "https://www.googleapis.com/auth/drive.file", "https://www.googleapis.com/auth/drive"]
creds = ServiceAccountCredentials.from_json_keyfile_name(PATH+'/sheets_cred.json', scope)
client = gspread.authorize(creds)


from flask import Flask,request
from datetime import datetime
app = Flask(__name__)
app.run(host='0.0.0.0',port=8)
@app.route('/arduino', methods=['GET', 'POST', 'PUT'])
def hello_world():
    #print('hello world')
    sensorValue=(str( request.get_json()))
    #print('This is standard output', file=sys.stdout)
    log = open("log.txt","a")
    log.write(str(datetime.now())+" -- "+sensorValue+"\n")
    sheet = client.open('iot_project')
    worksheet = sheet.get_worksheet(0)
    time = str(datetime.now())
    value = int(sensorValue)
    worksheet.insert_row((time, value), 2)


    return 'server ack',200

