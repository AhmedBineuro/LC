# LC
**This project was created as an anniversary gift for my girlfriend and also as a learning project to learn about HTTP requests, simple networking and package parsing.**
This is the simplest version of the messaging network using two ESP8266 nodes, one will run the server while the other will have a ST77335S 1.8 inch LCD display connected to it displaying the messages. Currently in order to send the messages to the recieving node, you will have to send an HTTP post request to the server with a body containing a JSON package containing two fields (id and msg). The ID is implemented for later upgrade to include multiple uniquely identified nodes and the message field is for the message.
# To implement:
- [] Create a captive portal to connect to wifi without hardcoding
- [] Multiple node system
- [] Using a webpage to send the messaging on each ESP8266 LC node
- [X] Simple ID system (no library)
- [] Server side dictionary to assign the messages to the correct node ID
- [] Ability to upload, send and display images
- [] Ability to add new nodes without hardcoding
- [] Saving messages on local SPIFFS file
