package application;

import java.net.URL;
import java.util.ResourceBundle;

import application.util.ImageLoader;
import application.util.SerialPortUtil;
import javafx.beans.property.BooleanProperty;
import javafx.beans.property.SimpleBooleanProperty;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;
import javafx.scene.layout.AnchorPane;
import javafx.stage.Stage;

/**
 * This class is the controller class of the DiscoBot GUI specified by
 * applicaton.view.RootLayout.fxml.
 * 
 * @author Doris Tang
 *
 */

public class RootLayoutController extends AnchorPane implements Initializable {

	private Stage stage;

	@FXML
	private Button upBtn;
	@FXML
	private Button downBtn;
	@FXML
	private Button leftBtn;
	@FXML
	private Button rightBtn;

	private BooleanProperty upPressed;
	private BooleanProperty downPressed;
	private BooleanProperty leftPressed;
	private BooleanProperty rightPressed;

	private SerialPortUtil portUtil;

	@Override
	public void initialize(URL location, ResourceBundle resources) {
		// insert icon images
		attachButtonGraphics();
		portUtil = new SerialPortUtil();
	}

	/**
	 * Sets the main application stage.
	 * 
	 * @param applicationStage
	 *            The main application {@link Stage}.
	 */
	public void setStage(Stage applicationStage) {
		stage = applicationStage;

		upPressed = new SimpleBooleanProperty(false);
		downPressed = new SimpleBooleanProperty(false);
		leftPressed = new SimpleBooleanProperty(false);
		rightPressed = new SimpleBooleanProperty(false);

		setButtonActions();
		bindKeyEventsToScene();
		addDirectionalBooleanListeners();
	}
	
	public void openPort() {
		portUtil.printSerialPortNames();
		portUtil.openPort();
	}

	public void testSerialPortWrite() {
		// change this for hardcoded testing/ connection
		portUtil.writeByteToPort(FORWARD);
	}
	
	public void closePort() {
		portUtil.closePort();
	}

	private void addDirectionalBooleanListeners() {
		upPressed.addListener(new ChangeListener<Boolean>() {
			@Override
			public void changed(ObservableValue<? extends Boolean> observable, Boolean oldValue, Boolean newValue) {
				if (newValue) {
					//portUtil.writeByteToPort(FORWARD);
					upBtn.setGraphic(ImageLoader.getUpInv());
				} else {
					upBtn.setGraphic(ImageLoader.getUp());
				}
			}
		});
		
		downPressed.addListener(new ChangeListener<Boolean>() {
			@Override
			public void changed(ObservableValue<? extends Boolean> observable, Boolean oldValue, Boolean newValue) {
				if (newValue) {
					//portUtil.writeByteToPort(FORWARD);
					downBtn.setGraphic(ImageLoader.getDownInv());
				} else {
					downBtn.setGraphic(ImageLoader.getDown());
				}
			}
		});
		
		leftPressed.addListener(new ChangeListener<Boolean>() {
			@Override
			public void changed(ObservableValue<? extends Boolean> observable, Boolean oldValue, Boolean newValue) {
				if (newValue) {
					//portUtil.writeByteToPort(FORWARD);
					leftBtn.setGraphic(ImageLoader.getLeftInv());
				} else {
					leftBtn.setGraphic(ImageLoader.getLeft());
				}
			}
		});
		
		rightPressed.addListener(new ChangeListener<Boolean>() {
			@Override
			public void changed(ObservableValue<? extends Boolean> observable, Boolean oldValue, Boolean newValue) {
				if (newValue) {
					//portUtil.writeByteToPort(FORWARD);
					rightBtn.setGraphic(ImageLoader.getRightInv());
				} else {
					rightBtn.setGraphic(ImageLoader.getRight());
				}
			}
		});
	}

	/**
	 * Sends the UP command to the serial port.
	 */
	private void doUpAction() {
		System.out.println("up");
		upPressed.set(!upPressed.get());
	}

	/**
	 * Sends the DOWN command to the serial port.
	 */
	private void doDownAction() {
		System.out.println("down");
		downPressed.set(!downPressed.get());
	}

	/**
	 * Sends the LEFT command to the serial port.
	 */
	private void doLeftAction() {
		System.out.println("left");
		leftPressed.set(!leftPressed.get());
	}

	/**
	 * Sends the RIGHT command to the serial port.
	 */
	private void doRightAction() {
		System.out.println("right");
		rightPressed.set(!rightPressed.get());
	}

	/**
	 * Binds a {@link KeyEvent} to its appropriate arrow button.
	 */
	private void bindKeyEventsToScene() {
		stage.getScene().setOnKeyPressed(new EventHandler<KeyEvent>() {
			@Override
			public void handle(KeyEvent event) {
				switch (event.getCode()) {
				case UP:
					upBtn.fire();
					break;
				case DOWN:
					downBtn.fire();
					break;
				case LEFT:
					leftBtn.fire();
					break;
				case RIGHT:
					rightBtn.fire();
					break;
				default:
					break;
				}
			}
		});
	}

	/**
	 * Sets the graphic of the buttons to the appropriate {@link ImageView}.
	 */
	private void attachButtonGraphics() {
		ImageLoader.loadImages();
		upBtn.setGraphic(ImageLoader.getUp());
		downBtn.setGraphic(ImageLoader.getDown());
		leftBtn.setGraphic(ImageLoader.getLeft());
		rightBtn.setGraphic(ImageLoader.getRight());
	}
	
	/**
	 * Sets the onAction property of arrow buttons.
	 */
	private void setButtonActions() {
		upBtn.setOnAction(new EventHandler<ActionEvent>() {
			@Override
			public void handle(ActionEvent event) {
				doUpAction();
			}
		});

		downBtn.setOnAction(new EventHandler<ActionEvent>() {
			@Override
			public void handle(ActionEvent event) {
				doDownAction();
			}
		});

		leftBtn.setOnAction(new EventHandler<ActionEvent>() {
			@Override
			public void handle(ActionEvent event) {
				doLeftAction();
			}
		});

		rightBtn.setOnAction(new EventHandler<ActionEvent>() {
			@Override
			public void handle(ActionEvent event) {
				doRightAction();
			}
		});
	}

	private final byte FORWARD = 0, BACKWARD = 1, FORWARD_LEFT = 2, FORWARD_RIGHT = 3, BACK_LEFT = 4, BACK_RIGHT = 5,
			SPIN_RIGHT = 6, SPIN_LEFT = 7, STOP = 8;

}
