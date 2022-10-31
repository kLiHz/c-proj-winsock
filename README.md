# C Project of Winsock

Implement a [server.c][s] and a [client.c][c] for file transfer. You should read and operate the .bin file named 'data'.

- The bin file contains an array of structs whose content is the transcript of 10 students.
- In [client.c][c] you should read the .bin file, sort the students by scores from highest to lowest and send the file to the server; [server.c][s] should receive the sorted data from [client.c][c] and convert it into a .csv file.
- You can use "127.0.0.1" as the IP for the server.
- "send()" can only transfer chars and strings, so you need convert the struct into chars.
- You should submit your code and screenshot of the content in the .csv file.
- The struct is shown as in [score.h](./score.h), you can copy and paste it in your server and client.
- Print the student number, name, score in the csv file.

[s]: ./server.c
[c]: ./client.c
