package application.util;

import java.util.Enumeration;

import gnu.io.CommPortIdentifier;
import gnu.io.SerialPort;

/**
 * This is a utility class that contains methods to write to a serial COM port.
 * 
 * @author Doris Tang
 */

@SuppressWarnings("unchecked")
public class SerialPortWriter {

	private static Enumeration<CommPortIdentifier> portList;
	private static CommPortIdentifier portId;
	static SerialPort serialPort;
	
	static {
		portList = CommPortIdentifier.getPortIdentifiers();
	}
	
	public static void printPortNames() {
		System.out.println("printing port names...");
		while (portList.hasMoreElements()) {
			portId = portList.nextElement();
			if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL && portId.getName().endsWith("usbmodem1411")) {
				System.out.println("found serial port "+portId.getName());
			}
		}
		System.out.println("finished printing port names");
	}
	
}