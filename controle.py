import keyboard
import serial
import time

porta = '/dev/ttyUSB0'
velocidade = 115200 

teclas_ativas = set()

try:
    arduino = serial.Serial(porta, velocidade, timeout=0.01)
    print("Conectando ao Arcade... aguarde.")
    
    time.sleep(2) 
    print("Conectado e pronto para o emulador! Pressione CTRL+C no terminal para sair.")

    while True:
        if arduino.in_waiting > 0:
            comando = arduino.readline().decode("utf-8", errors="ignore").strip()
            
            estado_atual = set(comando)

            teclas_para_pressionar = estado_atual - teclas_ativas
            teclas_para_soltar = teclas_ativas - estado_atual

            for t in teclas_para_pressionar:
                keyboard.press(t)

            for t in teclas_para_soltar:
                keyboard.release(t)

            teclas_ativas = estado_atual

        time.sleep(0.001)

except KeyboardInterrupt:
    print("\nCONEXÃO ENCERRADA PELO USUÁRIO")

except serial.SerialException as e:
    print(f"\nErro na Serial: {e}")    
    
finally:
    for t in teclas_ativas:
        keyboard.release(t)
        
    if 'arduino' in locals() and arduino.is_open:
        arduino.close()
        print("Porta serial fechada com segurança!")