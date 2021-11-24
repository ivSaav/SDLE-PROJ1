# SDLE Project1

## Usage

### Compile
Run `make` in the root folder. The executables will be generated in the `build/apps` directory.

### Run
- Run the Broker:

`./build/apps/broker`

- Run the TestApp:
    - *filename*: Configuration file with operations to be executed.
    - *node_id*: Client identification.

`./build/apps/test_app filename node_id`

### Dependencies
Make sure you have the following dependencies installed before running the project: 
- c++17
- g++ (GCC) 11.1.0
- [zmqpp](https://github.com/zeromq/zmqpp)
- [cereal](https://uscilab.github.io/cereal/)


### Configuration file
The configuration file contains the list of operations to be executed by 
one client. There are 5 possible operations: 
- `PUT topic_name number_of_puts message`
    - *topic_name*: Name of the topic the message refers to.
    - *number_of_puts*: Number of times to repeat the PUT operation.
    - *message*: Message to be put on queue.
- `GET topic_name number_of_gets`
    - *topic_name*: Name of the topic to get the message from.
    - *number_of_puts*: Number of times to repeat the GET operation.
- `SUB topic_name`
    - *topic_name*: Name of the topic to subscribe.
- `UNSUB topic_name`
    - *topic_name*: Name of the topic to unsubscribe.
- `SLEEP miliseconds_to_sleep`
    - *miliseconds_to_sleep*: Time in miliseconds to sleep.

Example of a simple configuration file: 
```
SUB topic1
PUT topic1 1 Hello World!
PUT topic1 1 World!
SLEEP 2000
GET topic1 2
UNSUB topic1
```

