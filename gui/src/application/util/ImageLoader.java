package application.util;

import java.io.IOException;
import java.io.InputStream;
import java.net.URL;

import javafx.scene.image.Image;
import javafx.scene.image.ImageView;

public class ImageLoader {

	private static ImageView up, down, left, right, up_inv, down_inv, left_inv, right_inv;

	public static void loadImages() {
		try {
			processImage(ImageLoader.class.getResource(ENTRY_PREFIX + UP));
			processImage(ImageLoader.class.getResource(ENTRY_PREFIX + DOWN));
			processImage(ImageLoader.class.getResource(ENTRY_PREFIX + LEFT));
			processImage(ImageLoader.class.getResource(ENTRY_PREFIX + RIGHT));
			processImage(ImageLoader.class.getResource(ENTRY_PREFIX + UP_INV));
			processImage(ImageLoader.class.getResource(ENTRY_PREFIX + DOWN_INV));
			processImage(ImageLoader.class.getResource(ENTRY_PREFIX + LEFT_INV));
			processImage(ImageLoader.class.getResource(ENTRY_PREFIX + RIGHT_INV));
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
			case PATH_FROM_ROOT + UP_INV:
				up_inv = image;
				return;
			case PATH_FROM_ROOT + DOWN_INV:
				down_inv = image;
				return;
			case PATH_FROM_ROOT + LEFT_INV:
				left_inv = image;
				return;
			case PATH_FROM_ROOT + RIGHT_INV:
				right_inv = image;
				return;
			default:
				return;
			}
		}
	}
	
	public static ImageView getUpInv() {
		return up_inv;
	}
	
	public static ImageView getDownInv() {
		return down_inv;
	}
	
	public static ImageView getLeftInv() {
		return left_inv;
	}
	
	public static ImageView getRightInv() {
		return right_inv;
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
	private static final String UP = "up.png", DOWN = "down.png", LEFT = "left.png", RIGHT = "right.png",
			UP_INV = "up_inv.png", DOWN_INV = "down_inv.png", LEFT_INV = "left_inv.png", RIGHT_INV = "right_inv.png";
}
