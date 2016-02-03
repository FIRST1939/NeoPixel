// SerialExample.java

import java.util.Date;

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
//                    serial.write("CURRENT TIME: %s", new Date().toString());
//                    serial.write((byte) 13);
//                    serial.write('\n');

			serial.write('\n');

                }
                catch(IllegalStateException ex){
                    ex.printStackTrace();                    
                }
                
                // wait 1 second before continuing
                Thread.sleep(250);
            }
            
        }
        catch(SerialPortException ex) {
            System.out.println(" ==>> SERIAL SETUP FAILED : " + ex.getMessage());
            return;
        }
    }
}

