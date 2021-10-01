# Operating Systems Project
This repository contains codes, outputs and documentations for [CE424 (Operating Systems)](http://ce.sharif.edu/courses/98-99/2/ce424-1/) course project lectured in Spring 2020.

## Phases
This project is divided into three separate phases. Each phase description is provided in this section.

### Phase 0
In this phase, a kernel module is created, which can compute *Fibonacci* series up to a given input number. It also contains an API where user can enter the number. This API uses a device file to communicate with kernel module. 

### Phase 1
In this phase, in client side user enters a process ID and then API prints out some details about that process in different periods of time.

### Phase 2
In this phase we implemented BLP logic using kernel modules. BLP is a **mandatory access control** method which defines some security labels to control read/write access of each file. Also access history of each file is saved in another file.

## Get Started
*  Open terminal and write following command:
    ```
    git clone https://github.com/alirezadaghigh99/Operating-Systems.git
    ```
    Then go to one of the phases and then run these following commands:
    
    ```
    make
    ```
    By executing this command, you will see some new files added to project. Then: 
    ```
    sudo insmod X.ko
    ```
    Where X is name of ko file.
    Then for monitoring the kernel messages use this command:
    ```
    sudo dmesg
    ```
    After that we have to give some permissions to this kernel module by following command
    ```
    sudo chmod 666 /dev/kernel-module-name

    ```
    After all we have to run client code by going to client folder and running this command:
    ```
    gcc -o api api.c
    ./api
    ```
    Again for monitoring kernel messages:
    ```
    sudo dmesg
    ```
    In the end, for removing this kernel module run this command
    ```
    sudo rmmod X.ko
    ```
    Where X is name of ko kernel module.

## Authors
- Nima Jamali
    - GitHub: [@nimajam41](https://github.com/nimajam41)
    - LinkedIn: [Nima Jamali](https://www.linkedin.com/in/nima-jamali-5b1521195/)

- Alireza Daghigh
    - GitHub: [@alirezadaghigh99](https://github.com/alierzadaghigh99)

This project is available thanks to all contributors.

