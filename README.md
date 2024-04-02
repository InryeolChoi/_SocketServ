# SocketServ
한국어 버전은 이 링크를 참조해주세요 : [클릭!](./README_ko.md) 
Please read this page if you can speak English

## Purpose
1. To gain a detailed understanding of networking through socket programming.
2. Preparation for backend development in future web programming.
3. Expected to be helpful in studying server and infrastructure.

## references
* book : 윤성우, 윤성우의 열혈 TCP/IP 소켓 프로그래밍
	* focused on Linux / Unix Socket Programming
* man pages about `kqueue()` : [link](https://man.freebsd.org/cgi/man.cgi?query=kqueue&sektion=2)

## Prerequisites
* Low-level C language knowledge.
* Linux/Unix and terminal (shell) usage.
* Multiprocessing and multithreading.
* File descriptors.

## Notes
* Codes written on here only run on Linux/Unix-based systems.
* They are not executable on Windows.

# parts
<Part 1: Networking and Sockets>
1. Understanding Network Programming and Sockets
2. Types of Sockets and Protocol Settings
3. Address System and Data Alignment
4. TCP-based Server/Client (1)
5. TCP-based Server/Client (2)
6. Graceful Disconnection of Sockets
7. Various Socket-related Functions
8. Socket-related Options

<Part 2: Multiplexing>
1. What is I/O Multiplexing?
2. Practice with epoll() - Runs on Linux
3. Practice with kqueue() 1 - macOS and FreeBSD
4. Practice with kqueue() 2 - macOS and FreeBSD
5. Building a Real HTTP Server
