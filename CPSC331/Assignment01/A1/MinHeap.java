/**
 * @author John Abo
 * Student ID: 30088517
 * 
 * MinHeap class that implements the methods from interface given in class.
 *
 * Pseodocode from lecture slides were used when implementing. Modified to
 * account for Java's indices starting at 0
 *
 * I wonder how exactly they want us to go about documentation. I'll probably
 * stick to the software engineering practices I've been taught (until
 * otherwise stated).
 */

package ca.ucalgary.cpsc331;
import ca.ucalgary.cpsc331.PriorityQueue;

public class MinHeap implements PriorityQueue {

	private int[] keys;
	private int length;
	private final int CAPACITY = 31;
	
	/**
	 * Constructor
	 */
	public MinHeap() {
		keys = new int[CAPACITY];
		length = 0;
	}

	/**
	 * Checks if the heap is empty by checking the length
	 */
	public boolean empty() {
		if (length == 0) {
			return true;
		}
		return false;
	}
	
	/**
	 * Checks if the heap is full by checking the length
	 */
    public boolean full() {
		if (length == CAPACITY) {
			return true;
		}
		return false;
	}
	
	/**
	 * Inserts a key into the end of heap, recursively inserts it into the right
	 * position
	 */
    public void insert(int key) {
		
		//If the heap is full, nothing should be inserted and user is notified
		if (full()) {
			throw new RuntimeException("Heap must be non-empty");
		}
		
		//Sets last element to largest possible value
		keys[length] = Integer.MAX_VALUE;
		length++;
		
		//Attempts to decrease key at length - 1
		decreaseKey(length - 1, key);
		
	}
	
	/**
	 * Should pretty much be the same as min, but also removes the key, then
	 * min-heapifies the array again. So that the other preconditions hold.
	 *
	 * Precondition:	keys is a non-empty array
	 */
    public int extractMin() {
		int min = keys[0];
		
		if (length == 0) {
			throw new RuntimeException("Heap must be non-empty");
		}
		
		keys[0] = keys[length-1];
		length--;
		
		minHeapify(0);
		
		return min;
	}
	
	/**
	 * In theory, this should only need to return the first element of the
	 * keys arrays
	 *
	 * Precondition:	keys is a non-empty array
	 */
    public int min() {
		if (length == 0) {
			throw new RuntimeException("Heap must be non-empty");
		}
		return keys[0];
	}
	
	/**
	 * Gets the length of the min-heap
	 */
	public int getLength() {
		return length;
	}

	/**
	 * Returns a copy of the min-heapified array
	 */
	public int[] getKeys() {
		
		int[] ret = new int[CAPACITY];
		
		for (int i = 0; i < CAPACITY; i++) {
			ret[i] = keys[i];
		}
		
		return ret;
	}
	
	/**
	 * Returns a string representation of the heap
	 */
	@Override
	public String toString() {
		String str = "";
		
		str += "size = " + length;
		
		int j = 0;
		for (int i = 1; i <=  (CAPACITY * 2) + 1; i *= 2) {
			
			str += "\n";
			
			for (int k = 0; j < length && k < i; k++) {
				
				str += keys[j] + " ";
				j++;
			}
			
			//removes excess space lol
			str = str.substring(0, str.length() - 1);
		}
		
		return str;
	}
	
	/**
	 * Min-heapifies the array
	 *
	 * Precondition:	i is an index of keys array, so that the binary tree
	 *					rooted at both left(i) and right(i) are min-heaps
	 *
	 * Should be done?
	 */
	private void minHeapify(int i) {
		if (i > length) {
			throw new RuntimeException("Index is outside of the heap");
		}
		
		int min, minIndex;
		
		//Gotta get the min of current and children lol
		
		min = keys[i];
		minIndex = i;
		
		if (left(i) < length) {	//Makes sure the left child exists first
			if(keys[left(i)] < min) {
				min = keys[left(i)];
				minIndex = left(i);
			}
		}
		
		if (right(i) < length) {	//Makes sure the right child exists first
			if(keys[right(i)] < min) {
				min = keys[right(i)];
				minIndex = right(i);
			}
		}
		
		//Swaps the elements and recursively calls if there was a swap
		if (i != minIndex) {
			
			//Swaps the elements if a new min is found
			int temp = keys[i];
			keys[i] = keys[minIndex];
			keys[minIndex] = temp;
			
			minHeapify(minIndex);
		}
	}
	
	/**
	 * Turns the keys array into a heap
	 */
	public void buildMinHeap() {
		int n = length;
		
		for (int i = (n/2); i >= 0; i--) {
			minHeapify(i);
		}
	}
	
	/**
	 * Decreases the specific key and re-heapifies the array
	 *
	 * Precondition:	i is the index of an element in min heap keys
	 *					key >= keys[i]
	 */
	private void decreaseKey(int i, int key){
		if (i > length) {
			throw new RuntimeException("Index i must be an element of heap");
		}
		
		if (key > keys[i]) {
			throw new RuntimeException("Key passed must be less than key already at i\nkeys:" + key + ", " + keys[i]);
		}
		
		keys[i] = key;
		
		while (i > 0 && keys[parent(i)] > keys[i]) {
			int temp = keys[parent(i)];
			keys[parent(i)] = keys[i];
			keys[i] = temp;
			
			i = parent(i);
		}
	}
	
	/**
	 * Gets the index of parent node
	 */
	private int parent(int i) {
		return (i - 1)/2;
	}
	
	/**
	 * Gets the index of left child node
	 */
	private int left(int i) {
		return (2 * i) + 1;
	}
	
	/**
	 * Gets the index of right child node
	 */
	private int right(int i) {
		return (2 * i) + 2;
	}
}