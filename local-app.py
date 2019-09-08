from flask import Flask,request
from datetime import datetime
import os
app = Flask(__name__)
app.run(host='0.0.0.0',port=80)
@app.route('/arduino', methods=['GET', 'POST', 'PUT'])
def hello_world():
    #print('hello world')
    sensorValue=(str( request.get_json()))
    print("SensorValue is : "+sensorValue)
    #print('This is standard output', file=sys.stdout)
    log = open("log.txt","a")
    log.write(str(datetime.now())+" -- "+sensorValue+"\n")
    if int (sensorValue) <60:
        print (str(len(os.popen("ps aux | grep afplay ").readlines())))
        if len(os.popen("ps aux | grep afplay ").readlines())==2:
            os.system("afplay ~/yuval3.mp3&")
    else:
        os.system("pkill afplay")
    #print ("hello")
    return 'server ack',200
