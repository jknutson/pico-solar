# Raspberry Pi Pico 2040 ADC Readings

Sometimes I get odd values displayed on the LCD display connected to my pico. It would be a positive or negative number ending in e.g. `E+16`. Such large nnumbers are not expected in my application so I used OpenOCD, and GDB to debug the code running on the pico in realtime over SWD. SWD is similar to JTAG.

set (list) breakpoints:
```
(gdb) info breakpoints
Num     Type           Disp Enb Address    What
2       breakpoint     keep y   0x10000410 in main at /home/pi/pico/pico-solar/main.c:68
        stop only if result < 0
        breakpoint already hit 1 time
3       breakpoint     keep y   0x10000410 in main at /home/pi/pico/pico-solar/main.c:68
        stop only if result > 100
```

breakpoint hit:
```
(gdb) list
63        char lcd_text[MAX_LINES][MAX_CHARS];
64        char prev_lcd_text[MAX_LINES][MAX_CHARS];
65        for (;;) {
66          // Read ADC
67          float result = sampled_adc_read(ADC_NUM_SAMPLES);
68   -->    float result_v = result * ADC_CONVERSION_FACTOR;
69          float voltage = voltage_divider(result_v, 10000.0, 2000.0);
70          sprintf(lcd_text[1], "V: %.3fv", voltage);
71
72          // Read DHT
(gdb) p result
$8 = -7.37869774e+18
```

stepping into `sampled_adc_read`:
```
(gdb) break ohms_law.c:10 if sample_sum < 0
Breakpoint 4 at 0x10000742: file /home/pi/pico/pico-solar/ohms_law.c, line 10.
(gdb) break ohms_law.c:10 if sample_sum >
Display all 200 possibilities? (y or n)
(gdb) break ohms_law.c:10 if sample_sum > 10000
Note: breakpoint 4 also set at pc 0x10000742.
Breakpoint 5 at 0x10000742: file /home/pi/pico/pico-solar/ohms_law.c, line 10.
(gdb) c
Continuing.
target halted due to debug-request, current mode: Thread
xPSR: 0x01000000 pc: 0x00000178 msp: 0x20041f00

Thread 1 hit Breakpoint 4, sampled_adc_read (sample_cnt=sample_cnt@entry=5) at /home/pi/pico/pico-solar/ohms_law.c:10
10          sample_sum += adc_read();
(gdb) p sample_sum
$9 = -3.68934881e+19
```
