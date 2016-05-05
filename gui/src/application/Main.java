package application;

import java.io.IOException;

import javafx.application.Application;
import javafx.event.EventHandler;
import javafx.fxml.FXMLLoader;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.layout.AnchorPane;

/**
 * This class is responsible for initializing the entire JavaFx
 * {@link Application}.
 * 
 * @author Doris Tang
 *
 */

public class Main extends Application {

	private Scene primaryScene;
	private Stage primaryStage;
	private AnchorPane rootLayoutController;

	@Override
	public void start(Stage primaryStage) {
		System.out.println("start");

		this.primaryStage = primaryStage;
		primaryStage.setTitle("DiscoBot");
		initRootLayout();

		primaryStage.show();
	}

	private void initRootLayout() {
		// Load root layout from FXML file.
		FXMLLoader loader = new FXMLLoader();
		loader.setLocation(getClass().getResource("view/RootLayout.fxml"));

		rootLayoutController = new RootLayoutController();
		loader.setController(rootLayoutController);
		loader.setRoot(rootLayoutController);

		try {
			rootLayoutController = (AnchorPane) loader.load();

			primaryScene = new Scene(rootLayoutController);
			primaryStage.setScene(primaryScene);
			primaryStage.setResizable(true);
			primaryStage.centerOnScreen();

			Parent root = primaryScene.getRoot();
			for (Node node : root.getChildrenUnmodifiable())
				node.setStyle("-fx-focus-color: -fx-outer-border; -fx-faint-focus-color: transparent;");
			
			((RootLayoutController) rootLayoutController).setStage(primaryStage);
		} catch (IOException e) {
			System.out.println("could not initialize root layout.");
			e.printStackTrace();
		}

		primaryStage.setOnCloseRequest(new EventHandler<WindowEvent>() {
			@Override
			public void handle(WindowEvent event) {
				System.out.println("exiting...");
			}
		});
	}

	public static void main(String[] args) {
		launch(args);
	}
}
