import customtkinter as ctk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import matplotlib.pyplot as plt
import pandas as pd
from PIL import Image, ImageTk
import threading
import time
import plot
import serial
from logger_config import setup_logger

logger = setup_logger()

default_port = "COM4"
# df = plot.get_initial_df()
data = list()
ser = None

def init_serial():
    global ser
    if ser is not None:
        ser.write("IDLE\n".encode('utf-8'))
        ser.close()
    port = port_variable.get()
    try:
        baud = int(baud_variable.get())
    except ValueError:
        logger.error(f"No se ingreso un valor valido de baud rate: {baud_variable.get()}")
    logger.info(f'Values from inputs PORT:{port}. BAUD: {baud}')
    try:
        ser = serial.Serial(port, baud, timeout=0.5)
        ser.write("ACTIVE\n".encode('utf-8'))
    except Exception as e:
        logger.error(f"Init serial {port} {baud}: {e}")

def close_serial():
    global ser
    if ser is not None:
        ser.write("IDLE\n".encode('utf-8'))
        ser.close()
        logger.info("Serial closed")
    
def toggle_sidebar():
    if sidebar_frame.winfo_viewable():
        sidebar_frame.grid_remove()
    else:
        sidebar_frame.grid()

def update_plot():
    while True:
        
        if ser is not None and (value := plot.read_serial(ser)):
            if value := plot.format_lidar_data(value):
                row = plot.format_df(value)
                logger.debug(f"Data: {row['distance']}")
                data.append(row)
        else:
            # logger.debug("No data")
            time.sleep(0.3)
            continue

        df = pd.DataFrame(data)
        if df.empty:
            time.sleep(1)
            continue
        angles = df["angle"]
        radii = df["distance"]
        ax.clear()
        ax.plot(angles, radii)
        ax.set_title("Dynamic Polar Plot", va='bottom')
        canvas.draw()
        time.sleep(1)


# Initialize the main CustomTkinter window
ctk.set_appearance_mode("System")  # Modes: "System", "Dark", "Light"
ctk.set_default_color_theme("blue")  # Themes: "blue", "green", "dark-blue"

root = ctk.CTk()
root.title("Proyecto Digitales II - Pablo Gomez")
root.geometry("1920x1200")

port_variable = ctk.StringVar(value=default_port)
baud_variable = ctk.IntVar(value=9600)


# Header
header_frame = ctk.CTkFrame(root, height=60, corner_radius=0)
header_frame.grid(row=0, column=0, columnspan=2, sticky="nsew", ipadx=8, ipady=8)

# Burger menu button to toggle sidebar
burger_button = ctk.CTkButton(header_frame, text="☰", command=toggle_sidebar, width=40, corner_radius=10)
burger_button.pack(side="left", padx=16)

# Add an image to the header
image = Image.open("logo.jpg").resize((40, 40))
header_image = ctk.CTkImage(light_image=image, dark_image=image)
header_label = ctk.CTkLabel(header_frame, image=header_image, text="", fg_color="transparent")
header_label.pack(side="left", padx=10)

# Title in the header
title_label = ctk.CTkLabel(header_frame, text="Sensor laser", font=("Arial", 16))
title_label.pack(side="left")


# Sidebar
sidebar_frame = ctk.CTkFrame(root, width=200, corner_radius=0)
sidebar_frame.grid(row=1, column=0, sticky="nsew")

sidebar_label = ctk.CTkLabel(sidebar_frame, text="Sidebar", font=("Arial", 14))
sidebar_label.pack(pady=12)

input_1 = ctk.CTkEntry(sidebar_frame, placeholder_text="Pasos por lectura")
input_1.pack(pady=4, padx=16)
button1 = ctk.CTkButton(sidebar_frame, text="Submit")
button1.pack(pady=4, padx=16)

# entry2 = ctk.CTkEntry(sidebar_frame, placeholder_text="Input 2")
# entry2.pack(pady=4, padx=16)
input_baud = ctk.CTkEntry(sidebar_frame, placeholder_text="Baud rate:", textvariable=baud_variable)
input_baud.pack(pady=4, padx=16)
input_port = ctk.CTkEntry(sidebar_frame, placeholder_text="Port:", textvariable=port_variable)
input_port.pack(pady=4, padx=16)

button2 = ctk.CTkButton(sidebar_frame, text="Iniciar", command=init_serial)
button2.pack(pady=4, padx=16)
button3 = ctk.CTkButton(sidebar_frame, text="Frenar")
button3.pack(pady=4, padx=16)

# Main content
main_frame = ctk.CTkFrame(root)
main_frame.grid(row=1, column=1, sticky="nsew")

main_title_label = ctk.CTkLabel(main_frame, text="Polar Plot", font=("Arial", 14))
main_title_label.pack(pady=10)

# Matplotlib Polar Plot
fig, ax = plt.subplots(subplot_kw={'projection': 'polar'})
canvas = FigureCanvasTkAgg(fig, master=main_frame)
canvas.get_tk_widget().pack(fill="both", expand=True)

# Configure grid layout
root.rowconfigure(1, weight=1)
root.columnconfigure(1, weight=1)




if __name__ == '__main__':
    # Start a thread to update the plot
    plot_thread = threading.Thread(target=update_plot, daemon=True)
    plot_thread.start()

    # Start the CustomTkinter main loop
    try:
        root.mainloop()
    except KeyboardInterrupt:
        pass
    finally:
        close_serial()
