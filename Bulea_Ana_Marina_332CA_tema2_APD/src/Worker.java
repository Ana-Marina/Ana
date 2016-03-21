import java.util.HashMap;
import java.util.HashSet;

/**
 * Clasa ce reprezinta o solutie partiala pentru problema de rezolvat. Aceste
 * solutii partiale constituie task-uri care sunt introduse in workpool.
 */
class PartialSolution {
	String fileName; //nume document

	/**
	 * 
	 * @param fileName nume document
	 */
	public PartialSolution(String fileName) {
		this.fileName = fileName;
	}

	/**
	 * rezolva
	 */
	public void process() {
	}
}

class Solution {
	String fileName; //nume document
	float rank; //rank document
	int maximalLength; //lungimea maximala
	HashMap<Integer, Integer> appearances; //numarul de aparitii pentru o lungime
	HashSet<String> maximalWords; //cuvintele maximale

	public Solution() {
		appearances = new HashMap<Integer, Integer>();
		maximalWords = new HashSet<String>();
	}
}

/**
 * Clasa ce reprezinta un thread worker.
 */
class Worker extends Thread {
	WorkPool wp;

	public Worker(WorkPool workpool) {
		this.wp = workpool;
	}

	public void setWorkPool(WorkPool workpool) {
		this.wp = workpool;
	}

	public void run() {
		while (true) {
			PartialSolution ps = wp.getWork();
			if (ps == null)
				break;

			ps.process();
		}
	}
}
