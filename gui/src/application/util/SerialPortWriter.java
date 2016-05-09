package application.util;

import java.io.IOException;
import java.io.OutputStream;
import java.util.Enumeration;

import gnu.io.CommPortIdentifier;
import gnu.io.NoSuchPortException;
import gnu.io.PortInUseException;
import gnu.io.SerialPort;
import gnu.io.UnsupportedCommOperationException;

/**
 * This is a utility class that contains methods to write to a serial COM port.
 * 
 * @author Doris Tang
 */

@SuppressWarnings("unchecked")
public class SerialPortWriter {

	private static Enumeration<CommPortIdentifier> portList;
	private static CommPortIdentifier portId;
	private static SerialPort serialPort;
	private static OutputStream outputStream;

	private static final String SERIAL_PORT_ID = "/dev/ttyS85";
	// private static final String SERIAL_PORT_ID = "/dev/tty.usbmodem1411";
	// private static final String SERIAL_PORT_ID = "/dev/tty.Bluetooth-Incoming-Port";
	private final static int BAUD_RATE = 9600;
	private final static int CONNECTION_INIT_TIMEOUT = 1000;

	static {
		try {
			portId = CommPortIdentifier.getPortIdentifier(SERIAL_PORT_ID);
			serialPort = (SerialPort) portId.open("DiscoBotGUI", CONNECTION_INIT_TIMEOUT);
			serialPort.setSerialPortParams(BAUD_RATE, SerialPort.DATABITS_8, SerialPort.STOPBITS_1,
					SerialPort.PARITY_NONE);
			outputStream = serialPort.getOutputStream();

		} catch (NoSuchPortException nspe) {
			System.err.println("ERROR: no such port");

		} catch (PortInUseException piue) {
			System.err.println("ERROR: " + SERIAL_PORT_ID + " port currently in use");
		
		} catch (UnsupportedCommOperationException ucoe) {
			System.err.println("ERROR: unsupported comm operation");
			
		} catch (IOException ioe) {
			System.err.println("ERROR: IO exception while getting output stream from serial port");
		}
	}

	public static void printSerialPortNames() {
		portList = CommPortIdentifier.getPortIdentifiers();
		System.out.println("\nprinting port names...");
		while (portList.hasMoreElements()) {
			portId = portList.nextElement();
			if (portId.getPortType() == CommPortIdentifier.PORT_SERIAL) {
				System.out.println(portId.getName());
			}
		}
		System.out.println("finished printing port names\n");
	}
}