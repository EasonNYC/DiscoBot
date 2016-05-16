package application.util;

import jssc.SerialPort;
import jssc.SerialPortException;
import jssc.SerialPortList;

/**
 * This is a utility class that contains methods to write to a serial COM port.
 * 
 * @author Doris Tang
 */

// @SuppressWarnings("unchecked")
public class SerialPortUtil {

	private String[] portNames;
	private SerialPort port;

	private final String SERIAL_PORT_ID = "/dev/tty.usbserial";
	private final byte START_BYTE = 36; // dollar symbol
	private final byte STOP_BYTE = 42; // star symbol

	public SerialPortUtil() {
		portNames = SerialPortList.getPortNames();
	}

	/**
	 * Connects to serial port and sets appropriate connection params
	 */
	public void openPort() {
		port = new SerialPort(SERIAL_PORT_ID);

		if (port.isOpened()) {
			System.out.println("serial port currently opened");
		} else {
			try {
				port.openPort();
				System.out.println("opened port");
				port.setParams(SerialPort.BAUDRATE_9600, SerialPort.DATABITS_8, SerialPort.STOPBITS_1,
						SerialPort.PARITY_NONE);

			} catch (SerialPortException spe) {
				System.err.println("ERROR: serial port exception");
				spe.printStackTrace();
			}
		}
	}

	public void writeToPort() {
		byte[] msg = new byte[] { START_BYTE, 8, STOP_BYTE };

		try {
			if (port.writeBytes(msg)) {
				System.out.println("write successful");
			} else {
				System.out.println("write not successful");
			}

		} catch (SerialPortException e) {
			e.printStackTrace();
		}
	}

	public void closePort() {
		try {
			port.closePort();
			System.out.println("closed port");

		} catch (SerialPortException e) {
			e.printStackTrace();
		}
	}

	public void writeByteToPort(byte msg) {
		if (port != null) {
			try {
				port.writeByte(START_BYTE);
				port.writeByte(msg);
				port.writeByte(STOP_BYTE);

			} catch (SerialPortException e) {
				e.printStackTrace();
			}
		}
	}

	public void printSerialPortNames() {
		System.out.println("\nprinting port names...");
		for (int i = 0; i < portNames.length; i++) {
			System.out.println(portNames[i]);
		}
		System.out.println("finished printing port names\n");
	}
}