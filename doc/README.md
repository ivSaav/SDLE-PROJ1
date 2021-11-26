# SDLE Project 1

SDLE Project 1 for group T2G12.

Group members:

1. Ana Barros (up201806593@edu.fe.up.pt)
2. Ivo Saavedra (up201707093@edu.fe.up.pt)
3. João Martins (up201806436@edu.fe.up.pt)
4. Diogo Rosário (up201806582@edu.fe.up.pt)

## Usage

### Compile
Run `make` in the root folder. The executables will be generated in the `build/apps` directory.
There is also the option to compile with the debug option. This option adds log messages to output stream.

### Run
- Run the Broker:

`./build/apps/broker`

- Now we have 2 possible options: 
    - Run the TestApp (Runs several operations specified in a file):
        - *filename*: Configuration file with operations to be executed.
        - *node_id*: Client identification.

        `./build/apps/test_app filename node_id`
    - Run main (Runs a single operation specified in a program argument):
        - *node_id*: Client identification.
        - *sub topic*: Subscribe a topic.
        - *unsub topic*: Unsubscribe from a topic.
        - *put topic content*: Put content to topic.
        - *get topic*: Get message from topic.
        - *print*: Print broker's topic queue.

        `./build/apps/main node_id (sub topic)|(put topic content)|(unsub topic)|(get topic)|(print)`

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
