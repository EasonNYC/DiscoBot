package application.loaders;

import java.io.IOException;
import java.io.InputStream;
import java.net.URL;

import javafx.scene.image.Image;
import javafx.scene.image.ImageView;

public class ImageLoader {

	private static ImageView up, down, left, right;

	public static void loadImages() {
		try {
			URL urlUp = ImageLoader.class.getResource(ENTRY_PREFIX + UP);
			processImage(urlUp);
			
			URL urlDown = ImageLoader.class.getResource(ENTRY_PREFIX + DOWN);
			processImage(urlDown);
			
			URL urlLeft = ImageLoader.class.getResource(ENTRY_PREFIX + LEFT);
			processImage(urlLeft);
			
			URL urlRight = ImageLoader.class.getResource(ENTRY_PREFIX + RIGHT);
			processImage(urlRight);
			
		} catch (IOException ioe) {
			ioe.printStackTrace();
		}
	}
	
	public static void processImage(URL url) throws IOException {
		if (url != null) {
			InputStream input = url.openStream();
			ImageView image = new ImageView(new Image(input));
			
			String urlStr = url.getFile();
			urlStr = urlStr.substring(urlStr.indexOf("application/view/"));
			switch (urlStr) {
			case PATH_FROM_ROOT + UP:
				up = image;
				return;
			case PATH_FROM_ROOT + DOWN:
				down = image;
				return;
			case PATH_FROM_ROOT + LEFT:
				left = image;
				return;
			case PATH_FROM_ROOT + RIGHT:
				right = image;
				return;
			default:
				return;
			}
		}
	}

	public static ImageView getUp() {
		return up;
	}

	public static ImageView getDown() {
		return down;
	}

	public static ImageView getLeft() {
		return left;
	}

	public static ImageView getRight() {
		return right;
	}

	private static final String ENTRY_PREFIX = "/application/view/";
	private static final String PATH_FROM_ROOT = "application/view/";
	private static final String UP = "up.png", DOWN = "down.png", LEFT = "left.png", RIGHT = "right.png";
}
