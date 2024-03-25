# ZEPHYR - RTOS
This repository represents an example on how to use Zephyr rtos.

In this project the following has been covered: 

- redefining device tree bindings using app.overlay file
- external button interrupt on rising/falling edge
- timer interrupts
- creating threads.

The app has been developt for the stm32f3discovery board. To build the project you need to set up your environment for
zephyr app development (follow the steps on the official cite: https://docs.zephyrproject.org/latest/develop/getting_started/index.html).

To build the project run: west build -p always -b stm2f3_disco. To flash the code run west flash from your cmd.
