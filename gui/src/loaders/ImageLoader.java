package loaders;

import java.io.IOException;
import java.io.InputStream;
import java.net.URL;

import javafx.scene.image.Image;

public class ImageLoader {

	private static Image up, down, left, right;

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
			Image image = new Image(input);
			
			String urlStr = url.getFile();
			urlStr = urlStr.substring(urlStr.indexOf("wormguides"));
			switch (urlStr) {
			case PATH_FROM_ROOT + UP:
				setUp(image);
				return;
			case PATH_FROM_ROOT + DOWN:
				setDown(image);
				return;
			case PATH_FROM_ROOT + LEFT:
				setLeft(image);
				return;
			case PATH_FROM_ROOT + RIGHT:
				setRight(image);
				return;
			default:
				return;
			}
		}
	}

	public static Image getUp() {
		return up;
	}

	public static void setUp(Image up) {
		ImageLoader.up = up;
	}

	public static Image getDown() {
		return down;
	}

	public static void setDown(Image down) {
		ImageLoader.down = down;
	}

	public static Image getLeft() {
		return left;
	}

	public static void setLeft(Image left) {
		ImageLoader.left = left;
	}

	public static Image getRight() {
		return right;
	}

	public static void setRight(Image right) {
		ImageLoader.right = right;
	}

	private static final String ENTRY_PREFIX = "/application/view/";
	private static final String PATH_FROM_ROOT = "appication/view/";
	private static final String UP = "up.png", DOWN = "down.png", LEFT = "left.png", RIGHT = "right.png";
}
