# Poll Vs Select

## Table of Contents
[Description](#description)<br>
[About Multiplexing](#about-multiplexing)<br>
[Poll](#poll)<br>
[Select](#select)<br>
[The Signature Brawl](#the-signature-brawl)<br>
[Who Wins](#who-wins)

## Description
The purpose of this repo is to explain the differences and use cases of Poll and Select. Additionally, a poll example will be provided.

## About Multiplexing
Multiplexing is commonly used by networks to consolidate multiple signals into a common medium. This limits the amount of hardware needed to run a server with a hub. As one server can take one signal and process it for several other signals. If multiplexing was not implemented, additional hardware would be needed to process the data.

## Poll
Poll waits for a set of file descriptors to perform input or output (I/O). It works with an array of structures called "pollfd". Each one of these structures contains a file descriptor, an event that is expected to happen, and a returned event.
This version of multiplexing is very efficient in non-blocking file descriptors, as it leverages when the file descriptor should be sleeping or not. Otherwise, the amount of resources used by non-blocking file descriptors would be exponential.

## Select
Select works very similar to Poll. It is another way of performing multiplexing with non-blocking file descriptors to limit the amount of resources used. The biggest thing to note, is that select can only monitor file descriptors less than 1024 (FD_SETSIZE).

## The Signature Brawl
Select versus Poll. Both are similar in functions, perform the same tasks, and require the same setup. So, why use one over the other?

### Advantage Poll has over Select
Poll unlike select, can grow and shrink as needed. It is not a statically allocated array such as select. Additionally, Poll performs with a higher set of file descriptors. This makes it possible to monitor greater than 1024 and be efficient. In retrospect, it can be said that poll has a variable length that can both shrink and grow as needed, and is more efficient with larger sets of file descriptors.

### Advantage of Select over Poll
Select seems like it has already lost the fight against Poll, but that is not the case. Poll can perform multiplexing with higher numbers, but the speed of Select at values less than 1024 is much greater. Unlike Poll, select is also very portable. It is used on most systems and can be moved around easily. Atlast, the reason Select is performing faster than poll is its efficent use of meta-data in file descriptors. It uses at most three bits of data per file descriptor, while Poll uses 64 bits. This causes the performance to slow due to the amount of data needed to copy over.

## Who Wins
So, who wins? Is Select better or is Poll? Honestly, depends on the use case. Both in regards to threading and multiprocessing are slow. This is because multiplexing is a synchronous function. Cases that select would be better, would be when it is known that your communication chains will be less than 1024 and you know how many communication chains will be used. Additionally, if the program needs to perform on older systems, Select wins.

Poll would be used when its unknown how many communication chains will be needed. This is cause Poll has the ability to shrink and grow as needed. If its known that you will have a greater number of communication chains, poll is once again winning. Poll should only be implemented on newer systems rather than legacy systems.

|            Select                 |             Poll                  |
|-----------------------------------|-----------------------------------|
| - More portable and can be used on legacy systems | - Only available on newer systems |
| - Faster when used with less than 1024 file descriptors | - Can handle greater amount of file descriptors at a consistent rate|
| - Static allocation | - Can dynamically allocate |
| - Uses at most 3 bits per file descriptor | - Uses 64 bits per file descriptor|

## Example
#### Must have Sudo Privledges and be on UNIX computer as this uses UNIX Domain Sockets
<ol>
  <li> Run ./build.sh
  <li> The server is located in <code>/build/bin/poll_server</code>
      <ul>
        <li> To run, <code>./poll_server &ltUNIX_FILEPATH&gt</code>
        <li> The server should display that it is awaiting connections.
        <li> Upon a connection the server should display the amount of characters of a filename and the number of characters in the file.
       </ul>
  <li> The client is located in <code>/build/bin/poll_client</code>
    <ul>
      <li> To run, <code> ./poll_client &ltUNIX_FILEPATH&gt &ltFILE&gt </code>
      <li> FILE is the file of which you want to grab a spiecific amount of characters from.
      <li> The client should say connected and then display if it failed or succeeded. Then close.
    </ul>
</ol>
      
