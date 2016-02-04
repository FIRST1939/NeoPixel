/*
 * ServiceTest
 *
 * A test harness for testing basic services provided by the
 * NeoPixel1939 Java class designed to drive NeoPixels using
 * a Teensy-LC running the TeensyPixel1939 firmware.
 */

import com.pi4j.io.serial.Serial;
import com.pi4j.io.serial.SerialDataEvent;
import com.pi4j.io.serial.SerialDataListener;
import com.pi4j.io.serial.SerialFactory;
import com.pi4j.io.serial.SerialPortException;

public class ServiceTest {

    public static void main(String args[]) throws InterruptedException {

        // instantiate the serial communications class

        final Serial serial = SerialFactory.createInstance();

        // Create and register the serial data listener which, for now, just echos anything
        // received back from the Teensy to the console.

        serial.addListener(new SerialDataListener() {
            @Override
            public void dataReceived(SerialDataEvent event) {
                System.out.print(event.getData());
            }
        });

        // Now try a call to the health-check service

        try {

            // open the default serial port provided on the GPIO header

            serial.open(Serial.DEFAULT_COM_PORT, 38400);

            // and call the health-check service

            for (int i = 0; i < 3; i++) {

                try {
                    serial.write('\n');
                } catch(IllegalStateException ex){
                    ex.printStackTrace();
                    System.exit(1);
                } // health check call try-catch

            }

        } catch(SerialPortException ex) {

            System.out.println("Error: Could not configure the UART on the GPIO port" + ex.getMessage());
            System.exit(1);

        } // serial config try-catch

    } // main() method

} // ServiceTest class
