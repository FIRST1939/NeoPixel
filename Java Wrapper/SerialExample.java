/*
 * SerialExample
 *
 * A basic bi-directional serial interface to the Teensy-LC NeoPixel driver that leverages the
 * Pi4J Raspberry Pi GPIO library.  See http://pi4j.com for detailed information on this excellent
 * driver and http://pi4j.com/example/serial.html for specific information on its UART support
 * capabilities.
 */

import com.pi4j.io.serial.Serial;
import com.pi4j.io.serial.SerialDataEvent;
import com.pi4j.io.serial.SerialDataListener;
import com.pi4j.io.serial.SerialFactory;
import com.pi4j.io.serial.SerialPortException;

public class SerialExample {

    public static void main(String args[]) throws InterruptedException {

        System.out.println("<--Pi4J--> Serial Communication Example ... started.");
        System.out.println(" ... connect using settings: 38400, N, 8, 1.");
        System.out.println(" ... data received on serial port should be displayed below.");

        // create an instance of the serial communications class
        final Serial serial = SerialFactory.createInstance();

        // create and register the serial data listener
        serial.addListener(new SerialDataListener() {
            @Override
            public void dataReceived(SerialDataEvent event) {
                // print out the data received to the console
                System.out.print(event.getData());
            }
        });

        try {

            // open the default serial port provided on the GPIO header
            serial.open(Serial.DEFAULT_COM_PORT, 38400);

            // continuous loop to keep the program running until the user terminates the program
            while(true) {

                try {

                    // Other examples of 'serial.write()':
                    //
                    // serial.write("CURRENT TIME: %s", new Date().toString());
                    // serial.write((byte) 13);
                    // serial.write('\n');
                    // serial.writeln("Hello World");

                    serial.write('\n');

                } catch(IllegalStateException ex){
                    ex.printStackTrace();
                }

                Thread.sleep(250); // sleep a bit
            }

        } catch(SerialPortException ex) {
            System.out.println(" ==>> SERIAL SETUP FAILED : " + ex.getMessage());
            return;
        }
    }
}
