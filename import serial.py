import serial
import csv
from datetime import datetime

PORT = "/dev/cu.usbmodem34B7DA65F2D82"
BAUDRATE = 115200

def main():
    # Crear nombre único para cada ejecución
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    output_file = f"microfonos_{timestamp}.csv"

    ser = serial.Serial(PORT, BAUDRATE, timeout=1)

    with open(output_file, mode="w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["pc_timestamp", "mic0", "mic1"])

        print(f"Grabando datos en {output_file}. Ctrl+C para parar.")

        try:
            while True:
                line = ser.readline().decode("utf-8", errors="ignore").strip()
                if not line:
                    continue

                parts = line.split(",")
                if len(parts) != 2:
                    print("Línea rara, la salto:", line)
                    continue

                mic0 = parts[0]
                mic1 = parts[1]

                now = datetime.now().isoformat(timespec="milliseconds")

                writer.writerow([now, mic0, mic1])
                f.flush()

                print(now, mic0, mic1)

        except KeyboardInterrupt:
            print("\nGrabación detenida.")
        finally:
            ser.close()

if __name__ == "__main__":
    main()
