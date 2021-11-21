# SDLE PROJ1 README 

## Run Instructions
In order to run our app, you must run the broker first. After this, you can run the testapp with your configuration file as an input, as well as the node id.

## Program specifics

During our testing, the application handled sending and receiving 100000 messages in around 9 seconds.

## Configuration file format
Configuration file has 5 possible operations listed below.

### PUT
PUT topic_name number_of_puts message

### GET
GET topic_name number_of_gets

### SUB
SUB topic_name

### UNSUB
UNSUB topic_name

### SLEEP
SLEEP miliseconds_to_sleep

