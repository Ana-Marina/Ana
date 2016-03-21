import java.util.LinkedList;

/**
 * Task de reduce
 */
class ReducePartialSolution extends PartialSolution {
	//lista cu solutiile de la map
	//la final se goleste si se pune rezultatul de la reduce
	LinkedList<Solution> solutions;

	/**
	 * 
	 * @param fileName nume document
	 * @param solutions input: solutii map; output: solutie reduce
	 */
	public ReducePartialSolution(String fileName, LinkedList<Solution> solutions) {
		super(fileName);
		this.solutions = solutions;
	}

	/**
	 * calculeaza al n-lea numar fibonacci
	 * @param n
	 * @return
	 */
	int fibonacci(int n) {
		int a = 0, b = 1, temp = 0;
		
		while (n > 0) {
			temp = a;
			a = b;
			b += temp;
			n--;
		}

		return a;
	}

	@Override
	public void process() {
		Solution solution = new Solution();
		int wordCount = 0; //numar de cuvinte
		Integer appearances;

		solution.rank = 0;
		solution.fileName = fileName;
		solution.maximalLength = 0;

		//etapa 1: combinare
		for (Solution s : solutions) {
			for (int length : s.appearances.keySet()) {
				wordCount += s.appearances.get(length);

				//insumeaza numarul de aparitii
				appearances = solution.appearances.get(length);
				if (appearances == null) {
					solution.appearances.put(length, 0);
					appearances = 0;
				}

				solution.appearances.put(length, appearances + s.appearances.get(length));

				//combina seturile de cuvinte maximale
				if (solution.maximalLength < s.maximalLength) {
					solution.maximalLength = s.maximalLength;
					solution.maximalWords.clear();
					solution.maximalWords.addAll(s.maximalWords);
				} else if (solution.maximalLength == s.maximalLength) {
					solution.maximalWords.addAll(s.maximalWords);
				}
			}
		}

		//etapa 2: procesare
		for (int length : solution.appearances.keySet()) {
			solution.rank += fibonacci(length + 1) * solution.appearances.get(length);
		}

		solution.rank = (int) ((solution.rank / wordCount) * 100) / (float) 100;

		solutions.clear();
		solutions.add(solution);
	}
}