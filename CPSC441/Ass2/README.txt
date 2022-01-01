Program was written on visual studio code, and tested be using
SSH extension on VSC to connect to the server and running there

To compile, run the makefile on any compatible system after
removing the .txt file extension.

Must first run the server with ./Ser then you can run
the client with ./Cli.

Running the client you must include command-line arguments:
<IP> <Port Server is running on>

This information is given upon running the server (if the
server doesn't bind, change the defined port in the code)

To select a service, enter the number prompted in the list
Translation
    - Only translates 5 words: Hello, Bye, Packet, Pain, Map
    - Only translates english to french
Convert
    - Can only convert between: CAD, USD, EUR, BTC, GBP
    - Any non-float value passed will result in this micro server crashing
Vote
    - Candidates are hard coded, and current results generated when running
    - (which means) I set the candidates and their results myself
    - The bonus was not done, sorry...