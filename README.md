/**

@file readme file
@brief Project 3 for ECEN5813 - Fall 2017
*In project 3 we were required to create a binary logger application building on prior work with the UART and Circular Buffers. The application also improves on previous memory manipulation functions to utilize DMA to move data. Additionally it creates profiling functions to measure the execution time of various memory manipulation functions. The profiler was integrated with the binary logger and  circular buffer, and real time clock in order to provide information to a terminal program such as a log ID, Timestamp, Log Length, Payload and a Checksum. The profiler retrieves the execution time of functions while the logger adds this time to a packet along with other status messages and outputs this information via UART to a terminal. In addition to the profiler and logger we also created an SPI interface in order to control a Nordic nRF24L01 module. We used created functions to read and write to specific registers on the Nordic chip in order to configure it and initiate wireless communication.

@author Andrew Kuklinski and Mason Darveaux
@date 12/8/2017 **/
