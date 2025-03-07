###############################################################################
 #
 # Copyright (C) 2022-2023 Maxim Integrated Products, Inc. (now owned by
 # Analog Devices, Inc.),
 # Copyright (C) 2023-2024 Analog Devices, Inc.
 #
 # Licensed under the Apache License, Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 #
 #     http://www.apache.org/licenses/LICENSE-2.0
 #
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.
 #
 ##############################################################################
import os
import argparse
import soundfile as sf
import numpy as np
import serial

def RX_Binary(ser, length = 16384):
    """
    Receives 16K binary samples for serial port
    """
    rx = np.empty(length, dtype=np.int8)
    cnt = 0
    while cnt < length:
        temp = ser.read()
        if temp:
            rx[cnt] = temp[0]
            cnt += 1
    return rx

def Capture_Serial(COMport, fname, length=16384):
    """
    Initialize the COM port, waits for  keyword 'START', then receives 16K binary samples,
    waits for 'END' and stores in a .npy file to be used for evaluation
    """
    ser = serial.Serial(COMport)
    ser.baudrate = 115200
    ser.timeout = 0.001

    print("waiting for serial data")
    while True:
        rx = ser.readline()
        line = rx[0:len(rx) - 1].decode(errors="ignore")
        if line == "":
            continue
        if line.startswith('Detected word') or line.startswith('LOW'):
            print(line)
        elif line.startswith('START'):
            # Receive binary
            data = RX_Binary(ser)
            print("Received serial data")
            continue
        elif line.startswith('END'):
            np.save(fname[:-4] + '.npy', data)
            ser.close()
            fname = fname[:-4] + '_' + str(counter) + '.wav'
            print(f"Saved serial data {data} to {fname}")
            sf.write(os.path.join("", fname), 100*data.astype('int16'), length)
            break
    return data

def command_parser():
    """
    Return the argument parser
    """
    parser = argparse.ArgumentParser(description='Captures serial bin file')
    parser.add_argument('-c', '--comport', type=str, required = False, default = 'COM12',
                        help='serial port')

    parser.add_argument('-o', '--output', type=str, required = False, default = 'out.wav',
                        help='output wav')
    
    return parser.parse_args()

if __name__ == "__main__":
    command = command_parser()
    counter = 0
    while True:
        data = Capture_Serial(COMport = command.comport, fname = command.output)
        counter += 1    

