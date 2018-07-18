# AESide


Inside ourcode/ folder lays client and server code that is able to simulate a server that expects a password (numeric key).

The client is able to perform a timing attack and discover, one by one, each character of the key. 

The server is vulnerable to the attack because comparison between the sent key and the real key returns an error as soon as a character does not match. Thus, the comparison that takes longer shall be the correct key. This concept is used to discover each character of the key one by one.
