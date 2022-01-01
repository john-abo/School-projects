/**
 *
 * Author:		John Abo
 * UCID:		30088517
 *
 * Implementation of a red-black tree data structure using algorithms given in
 * class and from text book. Dictionary interface was provided by the professor
 *
 */
package ca.ucalgary.cpsc331;

public class RedBlackTree implements Dictionary{
	private Node root;
	
	/**
	 * Checks if the tree is empty by checking for a root node
	 */
	public boolean empty() {
		return root == null;
	}
	
	/**
	 * Inserts node with key given into the tree then fixes RBT properties
	 *
	 * TODO
	 */
    public void insert(int key) {
		Node z = new Node(key);
		
		Node y = null;
		Node x = root;
		
		while (x != null) {
			y = x;
			
			if (z.getKey() < x.getKey()) {
				x = x.getLeftChild();
			} else {
				x = x.getRightChild();
			}
		}
		
		z.setParent(y);
		
		if (y == null) {
			root = z;
		} else if (z.getKey() < y.getKey()) {
			y.setLeftChild(z);
		} else {
			y.setRightChild(z);
		}
		
		z.setLeftChild(null);
		z.setRightChild(null);
		z.setColour("red");
		
		//There is where I'd call the fixup method
		//IF I HAD ONE
		insertFix(z);
	}
	
	/**
	 * Searches for node with key given, then removes node and fixes RBT
	 * properties
	 *
	 * TODO
	 */
    public void delete(int key) {
		Node z = getNode(root, key);	//Find node with given key
		
		if (z == null) {
			throw new RuntimeException("Node could not be found");
		}
		
		//Following code is written using pseudocode given in textbook
		
		Node y = z;
		String original = y.getColour();
		
		Node x;
		
		if (z.getLeftChild() == null) {	//Only the right child will be displaced
			x = z.getRightChild();
			transplant(z, z.getRightChild());
		} else if (z.getRightChild() == null) { //Only the left child will be displace
			x = z.getLeftChild();
			transplant(z, z.getLeftChild());
		} else {
			y = treeMinimum(z.getRightChild());	//Gets successor to z
			original = y.getColour();
			x = y.getRightChild();
			
			if (y.getParent() == z) {
				x.setParent(y);
			} else {
				transplant(y, y.getParent());
				y.setRightChild(z.getRightChild());
				y.getRightChild().setParent(y);
			}
			
			transplant(z, y);
			y.setLeftChild(z.getLeftChild());
			y.getLeftChild().setParent(y);
			y.setColour(z.getColour());
		}
		
		if (original.equalsIgnoreCase("black") && x != null) {
			deleteFix(x);
		}
	}
	
	/**
	 * Searches for node with key given, returns true if it exists, false otherwise
	 */
    public boolean member(int key) {
		
		//If getting the node returns null, there is no node with a key that
		//matches the one given
		if (getNode(root, key) == null) {
			return false;
		}
		
		return true;
	}
	
	/**
	 * Returns string representation of red-black tree
	 *
	 * TODO
	 */
	@Override
	public String toString() {
		String ret = "";
		
		//For now, lets just do an inorder traversal to print everything
		//Don't wanna deal with these requirements just yet
		
		//System.out.println("Bruh: " + ret.toString());
		
		if (!empty()) {
			ret += preorder(root, ret, "*");
			
			ret = ret.substring(0, ret.length() - 1);
		}
		
		return ret;
	}
	
	/**
	 * recursively creates a string representation of the red-black tree
	 */
	public String preorder(Node x, String line, String address) {
		if (x != null) {
			
			line = line.concat(address);
			
			line = line.concat(":");
			line = line.concat(x.getColour());
			
			line = line.concat(":");
			line = line.concat(Integer.toString(x.getKey()));
			
			line = line.concat("\n");
			
			line = preorder(x.getLeftChild(), line, address.concat("L"));
			
			line = preorder(x.getRightChild(), line, address.concat("R"));
		}
		
		//System.out.println("Bruh 1: \n" + line);
		
		return line;
	}
	
	/**
	 * Searches tree for node with a matching given key, returns the first one
	 * found with a key matching the one passed
	 */
	private Node getNode(Node targetRoot, int key) {
		if (targetRoot == null || key == targetRoot.getKey()) return targetRoot;
		
		if (key < targetRoot.getKey()) return getNode(targetRoot.getLeftChild(), key);
		
		else return getNode(targetRoot.getRightChild(), key);
	}
	
	/**
	 * Gets minimum of a subtree with root targetRoot
	 */
	private Node treeMinimum(Node targetRoot) {
		while (targetRoot.getLeftChild() != null) {
			targetRoot = targetRoot.getLeftChild();
		}
		
		return targetRoot;
	}
	
	/**
	 * Transplants patient node with child node
	 */
	private void transplant(Node patient, Node child) {
		if (patient.getParent() == null) {
			root = child;
		} else if (patient == patient.getParent().getLeftChild()) {
			patient.getParent().setLeftChild(child);
		} else {
			patient.getParent().setRightChild(child);
		}
		
		if (child != null) {
			child.setParent(patient.getParent());
		}
	}
	
	/**
	 * Performs the a left rotation of the subtree rooted at x
	 */
	private void leftRotate(Node x) {
		Node y = x.getRightChild();
		x.setRightChild(y.getLeftChild());
		
		if (y.getLeftChild() != null) {
			y.getLeftChild().setParent(x);
		}
		
		y.setParent(x.getParent());
		
		if (x.getParent() == null) {
			root = y;
		} else if (x == x.getParent().getLeftChild()) {
			x.getParent().setLeftChild(y);
		} else {
			x.getParent().setRightChild(y);
		}
		
		y.setLeftChild(x);
		x.setParent(y);
	}
	
	/**
	 * Performs the a right rotation of the subtree rooted at x
	 */
	private void rightRotate(Node x) {
		Node y = x.getLeftChild();
		x.setLeftChild(y.getRightChild());
		
		if (y.getRightChild() != null) {
			y.getRightChild().setParent(x);
		}
		
		y.setParent(x.getParent());
		
		if (x.getParent() == null) {
			root = y;
		} else if (x == x.getParent().getLeftChild()) {
			x.getParent().setLeftChild(y);
		} else {
			x.getParent().setRightChild(y);
		}
		
		y.setRightChild(x);
		x.setParent(y);
	}
	
	/**
	 * Restores RBT properties to the tree after a node was inserted
	 */
	private void insertFix(Node z) {
		
		Node y;
		
		while (z.getParent() != null && z.getParent().getColour().equalsIgnoreCase("red")) {
			
			if (z.getParent() == z.getParent().getParent().getLeftChild()) {
					
				//BUT WHAT IF THERE'S NO LEFT??????
				y = z.getParent().getParent().getRightChild();
				
				if (y != null && y.getColour().equalsIgnoreCase("red")) {
					z.getParent().setColour("black");
					y.setColour("black");
					z.getParent().getParent().setColour("red");
					z = z.getParent().getParent();
				} else {
					
					if (z == z.getParent().getRightChild()) {
						z = z.getParent();
						leftRotate(z);
						//z = z.getLeftChild(); This line and it's partner have scuffed my code so hard
					}
					
					z.getParent().setColour("black");
					z.getParent().getParent().setColour("red");
					rightRotate(z.getParent().getParent());
				}
			} else {
				
				//Pretty much copy paste top but change some stuff
				y = z.getParent().getParent().getLeftChild();
				
				if (y != null && y.getColour().equalsIgnoreCase("red")) {
					z.getParent().setColour("black");
					y.setColour("black");
					z.getParent().getParent().setColour("red");
					z = z.getParent().getParent();
				} else {
					if (z == z.getParent().getLeftChild()) {
						z = z.getParent();
						rightRotate(z);
						//z = z.getRightChild(); who the hell added this line?
					}
					
					z.getParent().setColour("black");
					z.getParent().getParent().setColour("red");
					leftRotate(z.getParent().getParent());
				}
			}
		}
		
		root.setColour("black");
	}
	
	/**
	 * Restores RBT properties to the tree after a node was deleted
	 */
	private void deleteFix(Node x) {
		
		Node w;
		
		while (x != root && x.getColour().equalsIgnoreCase("black")) {
			
			if (x == x.getParent().getLeftChild()) {
				
				w = x.getParent().getRightChild();
				
				System.out.println("w's key: " + w.getKey());
				
				if (w.getColour().equalsIgnoreCase("red")) {
					w.setColour("black");
					x.getParent().setColour("red");
					leftRotate(x.getParent());
					w= x.getParent().getRightChild();
				}
				
				if (w.getLeftChild().getColour().equalsIgnoreCase("black") &&
					w.getRightChild().getColour().equalsIgnoreCase("black")) {
						
					w.setColour("red");
					x = x.getParent();
				} else if (w.getRightChild().getColour().equalsIgnoreCase("black")) {
					w.getLeftChild().setColour("black");
					w.setColour("red");
					rightRotate(w);
					w = x.getParent().getRightChild();
					
					w.setColour(x.getParent().getColour());
					x.getParent().setColour("black");
					w.getRightChild().setColour("black");
					leftRotate(x.getParent());
					x = root;
				}
			} else {
				//Copy paste top and switch directions
				w = x.getParent().getLeftChild();
				
				System.out.println("w's key: " + w.getKey());
				
				if (w.getColour().equalsIgnoreCase("red")) {
					w.setColour("black");
					x.getParent().setColour("red");
					rightRotate(x.getParent());
					w= x.getParent().getLeftChild();
				}
				
				if (w.getRightChild().getColour().equalsIgnoreCase("black") &&
					w.getLeftChild().getColour().equalsIgnoreCase("black")) {
						
					w.setColour("red");
					x = x.getParent();
				} else if (w.getLeftChild().getColour().equalsIgnoreCase("black")) {
					w.getRightChild().setColour("black");
					w.setColour("red");
					leftRotate(w);
					w = x.getParent().getLeftChild();
					
					w.setColour(x.getParent().getColour());
					x.getParent().setColour("black");
					w.getLeftChild().setColour("black");
					rightRotate(x.getParent());
					x = root;
				}
			}
		}
		
		x.setColour("black");
	}
}

/**
 * Helper class that will constitute the nodes of the tree
 * All methods in this class run in constant time
 */
class Node {
	private int key;
	
	private String colour;
	
	private Node parent;
	private Node rightChild;
	private Node leftChild;
	
	public Node(int key) {
		this.key = key;
	
		this.parent = null;
		this.rightChild = null;
		this.leftChild = null;
	}
	
	/**
	 * Gets the key to the node
	 */
	public int getKey() {
		return key;
	}
	
	/**
	 * Paints the node either red or black
	 */
	public void setColour(String colour) {
		this.colour = colour;
	}
	
	public String getColour() {
		return colour;
	}
	
	public void setLeftChild(Node left) {
		this.leftChild = left;
	}
	
	public void setRightChild(Node right) {
		this.rightChild = right;
	}
	
	public void setParent(Node parent) {
		this.parent = parent;
	}
	
	/**
	 * In the case of this getter, we actually do want to return the address
	 * of the Node, as we are treating it as a pointer
	 */
	public Node getLeftChild() {
		return leftChild;
	}
	
	/**
	 * In the case of this getter, we actually do want to return the address
	 * of the Node, as we are treating it as a pointer
	 */
	public Node getRightChild() {
		return rightChild;
	}
	
	public Node getParent() {
		return parent;
	}
}
