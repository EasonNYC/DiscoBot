package application;

import java.net.URL;
import java.util.ResourceBundle;

import application.util.ImageLoader;
import application.util.SerialPortWriter;
import javafx.beans.property.BooleanProperty;
import javafx.beans.property.SimpleBooleanProperty;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
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

	@Override
	public void initialize(URL location, ResourceBundle resources) {
		// insert icon images
		attachButtonGraphics();
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
		
		// serial port stuff
		SerialPortWriter.printSerialPortNames();
	}

	private void addDirectionalBooleanListeners() {
		upPressed.addListener(new ChangeListener<Boolean>() {
			@Override
			public void changed(ObservableValue<? extends Boolean> observable, Boolean oldValue, Boolean newValue) {
				
			}
		});
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

	/**
	 * Sends the UP command to the serial port.
	 */
	private void doUpAction() {
		System.out.println("up");
	}

	/**
	 * Sends the DOWN command to the serial port.
	 */
	private void doDownAction() {
		System.out.println("down");
	}

	/**
	 * Sends the LEFT command to the serial port.
	 */
	private void doLeftAction() {
		System.out.println("left");
	}

	/**
	 * Sends the RIGHT command to the serial port.
	 */
	private void doRightAction() {
		System.out.println("right");
	}

	/**
	 * Binds a {@link KeyEvent} to its appropriate arrow button.
	 */
	private void bindKeyEventsToScene() {
		stage.getScene().setOnKeyPressed(new EventHandler<KeyEvent>() {
			@Override
			public void handle(KeyEvent event) {
				switch (event.getCode()) {
				case I:
					upPressed.set(true);
					break;
				case K:
					downPressed.set(true);
					break;
				case J:
					leftPressed.set(true);
					break;
				case L:
					rightPressed.set(true);
					break;
				default:
					break;
				}
			}
		});

		stage.getScene().setOnKeyReleased(new EventHandler<KeyEvent>() {
			@Override
			public void handle(KeyEvent event) {
				switch (event.getCode()) {
				case I:
					upPressed.set(false);
					break;
				case K:
					downPressed.set(false);
					break;
				case J:
					leftPressed.set(false);
					break;
				case L:
					rightPressed.set(false);
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

}
