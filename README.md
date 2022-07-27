# FlircProntoCodes

FlircProntoCodes is a command line utility for generating Pronto IR codes
for the Flirc TV IR receiver. You simply provide the device code and the 
command code for the desired keyboard command and you will get the Pronto
IR codes you need to activate it.

### Usage

FlircProntoCodes <em>DeviceCode</em> <em>CommandCode</em>

 - <em>DeviceCode</em>:  Hexidecmal code for the Flirc device
 - <em>CommandCode</em>:  Hexidecmal code for the Flirc command

 Generates the pronto hex codes to send an IR command to the Flirc TV USB receiver.

### Example

<code>C:\> FlircProntoCodes 02 0a</code>

<code>0000 006d 0000 0012 00ba 00ba 0020 0040 0020 0040 0020 0040 0020 0020 0020 0040 0020 0040 0020 0040 0020 00fa 0020 0020 0020 0040 0040 0020 0020 0020 0020 0020 0020 0020 0020 0040 0040 0040 0040 0020</code>

<code>C:\></code>
