package application;

import java.net.URL;
import java.util.ResourceBundle;

import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.Button;
import javafx.scene.layout.AnchorPane;
import application.loaders.ImageLoader;

/**
 * This class is the controller class of the DiscoBot GUI specified by
 * applicaton.view.RootLayout.fxml.
 * 
 * @author Doris Tang
 *
 */

public class RootLayoutController extends AnchorPane implements Initializable {

	@FXML
	private Button upBtn;
	@FXML
	private Button downBtn;
	@FXML
	private Button leftBtn;
	@FXML
	private Button rightBtn;

	@Override
	public void initialize(URL location, ResourceBundle resources) {
		attachButtonGraphics();
		addArrowButtonListeners();
	}

	/**
	 * Adds listeners for arrow buttons.
	 */
	private void addArrowButtonListeners() {
		
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
