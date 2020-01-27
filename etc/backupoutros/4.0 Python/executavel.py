import serial
import sqlite3  # só cria bd interno, então precisamos de outro para conectar ao bd remoto
import time
import datetime
import ipapi
import json
import schedule
import requests
from threading import Timer

def setInterval(function, interval, *params, **kwparams): # 1 função para fazer o intervalo de checagem
    def setTimer(wrapper):
        wrapper.timer = Timer(interval, wrapper)
        wrapper.timer.start()

    def wrapper():
        function(*params, **kwparams)
        setTimer(wrapper)

    setTimer(wrapper)
    return wrapper


def clearInterval(wrapper):     # 2 função para fazer o intervalo de checagem
    wrapper.timer.cancel()

##### BD LOCAL #######
ide = 0
data: float = 0
peso = ''
status = 0

##### RASP #######
long = 0
lat = 0
dataAtual = ''

##### ARDUINO #######
cod_ali = ''
peso_ali: float = 0
temp_ali = 0


def ip_long_lat(): # pega longidute e latitude
    global long
    global lat

    long = str(ipapi.location(None, None, 'longitude'))
    lat = str(ipapi.location(None, None, 'latitude'))
    print(str(long))
    print(str(lat))

def datahora():  # pega a data em A/M/D Hor/Min/Seg

    global dataAtual
    dataAtual = datetime.datetime.now()
    print(dataAtual.strftime("%Y-%m-%d %H:%M:%S"))
    print(dataAtual)

def update_task(conn, task): # função de update no banco local
    """
    update priority, begin_date, and end date of a task
    :param conn:
    :param task:
    :return: project id
    """
    sql = ''' UPDATE data
              SET status = ?
              WHERE unix = ?'''
    cur = conn.cursor()
    cur.execute(sql, task)

# def criando_db():  # criar DB LOCAL
#     conn = sqlite3.connect('horarios.db')
#     cursor = conn.cursor()
#     cursor.execute("""
#     CREATE TABLE data (
#             ide INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
#             unix TEXT NOT NULL,
#             peso FLOAT,
#             status BOOL
#     );
#     """)
#     print('Tabela criada com sucesso.')
#     conn.close()

# def adicionar_dados_bd_local(id_r, unix_r, peso_r, status_r): # adiciona os dados ao banco local
#     conn = sqlite3.connect('horarios.db') #BD local
#     cursor = conn.cursor()
#
#     # lista = [(1,'1565637515', 0.5, '0'),    # são os dados que vamos pegar no BD externo.
#     #          (2,'1565638333', 0.3, '0'),    # este aqui é apenas um exemplo para ver se o BD funciona
#     #          (3,'1565639999', 0.8, '0')]
#
#     cursor.execute(""" INSERT INTO data(ide, unix, peso, status)
#               VALUES(?,?,?,?) """, lista)
#     conn.commit()
#     conn.close()

def pegando_dados_bd_local(): # pega os dados do próximo horario da ração no banco local
    global ide
    global data
    global peso
    global status

    conn = sqlite3.connect('horarios.db')
    cursor = conn.cursor()
    cursor.execute("""
    SELECT * FROM data WHERE status = '0'
    """)

    dados = cursor.fetchall()

    print(dados)

    dado = dados[0]
    ide = dado[0]
    data = dado[1]
    peso = dado[2]
    status = dado[3]
    print(ide)
    print(data)
    print(peso)
    print(status)
    data = int(data)

    conn.close()

def pegando_dados_arduino():   # pegando os dados do arduino
    global cod_ali
    global peso_ali
    global temp_ali

    try:                       # necessário tratar, caso o arduino não esteja funcionando ou não esteja conectado
        port = "/dev/ttyACM0"
        s1 = serial.Serial(port, 9600)
        s1.flush()
        if s1.inWaiting() > 0:
            inputValue = s1.readline()
        resp = json.loads(inputValue)
        cod_ali = resp['cod']
        peso_ali = resp['peso']
        temp_ali = resp['temperatura']

    except:
        print("Não foi possível enviar mensagem ao arduino")


def enviar_dados_arduino():     # envia os dados de peso e aciona o arduino, modifica o status da refeição no BD local.
    global peso
    global data
    if time.time() > data:
        print("DATA:"+str(data))
        data = str(data)
        print("DATA2:" + data)
        try:                                    # necessário tratar, caso o arduino não esteja funcionando ou não esteja conectado
            port = "/dev/ttyACM0"
            s1 = serial.Serial(port, 9600)
            msgarduino = '{"envio":"despejar","quantidade":%s}' %peso
            s1.write(str(msgarduino))
            print("foi enviado para o arduino: " + str(msgarduino))
            conn = sqlite3.connect('horarios.db')
            with conn:
                update_task(conn, (1, data))
        except:
            print("Erro de comunicação com o arduino")


def enviar_nuvem(): # POST # qual modo vamos usar para enviar as informações no BD ou AWS?
    global cod_ali
    global peso_ali
    global temp_ali
    global long
    global lat
    dataAtual = datetime.datetime.now()

    API_ENDPOINT = "http://joao.com/x.php"
    API_KEY = "XXXXXXXXXXXXXXXXX"

    source_code = ''' 
    {       
            "cod": "%s"
            "peso": "%s",
            "long": "%s"
            "lat": "%s"
            "status": "%s"

        }
        ''' %(cod_ali, peso_ali, tempo_ali, long, lat, status)

    # data to be sent to api
    data = {'api_dev_key': API_KEY,
            'api_option': 'paste',
            'api_paste_code': source_code,
            'api_paste_format': 'python'}

    r = requests.post(url=API_ENDPOINT, data=data)

    pastebin_url = r.text
    print("The pastebin URL is:%s" % pastebin_url)
    print("")

def checar_nuvem(): # GET # '{"cod":"001","unix":"1565637515"}'
    URL = "http://joao.com/json"
    cod_ali = "A001"
    PARAMS = {'cod': cod_ali} # acho que usar o unix para selecionar a quantidade de dias.
    dados_nuvem = requests.get(url=URL, params=PARAMS)
    dados_json = dados_nuvem.json()
    id_x = dados_json['sequencia'] # aqui pegamos o primeiro id que veio do BD remoto
    for dados in dados_json:
        id_r = dados['cod']['horarios'][id_x]['xxxx']['id']
        unix_r = dados['cod']['horarios'][id_x]['xxxx']['unix']
        peso_r = dados['cod']['horarios'][id_x]['xxxx']['peso']
        status_r = dados['cod']['horarios'][id_x]['xxxx']['status']

        ##### insere no banco de dados #####
        conn = sqlite3.connect('horarios.db') #BD local
        cursor = conn.cursor()
        cursor.execute(""" INSERT INTO data(ide, unix, peso, status)
                   VALUES(s%,s%,s%,s%) """) %(id_r, unix_r, peso_r, status_r)
        conn.commit()
        conn.close()
        id_x += 1

#######################

schedule.every(1).days.at("07:00").do(checar_nuvem) # faz a checagem a cada 1 dia as 7:00 no BD
schedule.every(1).days.at("12:00").do(checar_nuvem)

ip_long_lat()
datahora()

while True:
    schedule.run_pending() # executa os eventos marcados

    pegando_dados_arduino() # o tempo de envio de uma informação do arduino para o rasp depende do código do arduino.

    time.sleep(1)
    interval_monitor = setInterval(enviar_dados_arduino, 60)  #tempo em segundos ele executa a função enviar_dados_arduino
    interval_monitor = setInterval(enviar_dados_arduino, 60)
    while True:
        pass

