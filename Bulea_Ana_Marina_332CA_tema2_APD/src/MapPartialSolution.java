import java.io.EOFException;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.ArrayList;
import java.util.LinkedList;

/**
 * Task de map
 */
class MapPartialSolution extends PartialSolution {
	//pozitie si dimensiune fragment
	int pos, fragmentSize;
	
	//lista in care se pune hashul fragmentului
	LinkedList<Solution> solutions;

	/**
	 * 
	 * @param fileName nume document
	 * @param pos pozitie fragment
	 * @param fragmentSize dimensiune fragment
	 * @param solutions lista in care se pune hashul fragmentului
	 */
	public MapPartialSolution(String fileName, int pos, int fragmentSize, LinkedList<Solution> solutions) {
		super(fileName);
		this.pos = pos;
		this.fragmentSize = fragmentSize;
		this.solutions = solutions;
	}

	/**
	 * @param b caracter
	 * @return true daca caracterul e spatiu alb
	 */
	boolean whitespace(byte b) {
		return ";:/?~\\.,><~`[]{}()!@#$%^&-_+'=*\"| \t\r\n".indexOf(b) != -1;
	}

	/**
	 * Proceseaza fragmentul
	 */
	@Override
	public void process() {
		ArrayList<Byte> fragment = new ArrayList<Byte>(); //octeti fragment
		RandomAccessFile file; //fisier citire
		
		byte fragmentBuffer[] = new byte[fragmentSize]; //buffer citire
		int fragmentStart = 0;
		byte b;
		String word;
		Solution solution = new Solution(); //output
		Integer appearances;

		try {
			file = new RandomAccessFile(fileName, "r");

			file.seek(pos);
			file.read(fragmentBuffer);

			//verifica daca fragmentul incepe in mijlocul unui cuvant
			if (pos > 0) {
				file.seek(pos - 1);

				if (!whitespace(file.readByte())) {
					//se duce la finalul cuvantului
					while (fragmentStart < fragmentSize) {
						if (whitespace(fragmentBuffer[fragmentStart]))
							break;
						fragmentStart++;
					}
				}
			}

			for (int i = fragmentStart; i < fragmentSize; i++)
				fragment.add(fragmentBuffer[i]);

			//verifica daca fragmentul se termina in mijlocul unui cuvants
			if (!whitespace(fragmentBuffer[fragmentSize - 1])) {
				file.seek(pos + fragmentSize);
				//citeste pana la finalul cuvantului
				try {
					while (true) {
						b = file.readByte();
						if (!whitespace(b))
							fragment.add(b);
						else
							break;
					}
				} catch (EOFException e) {

				}
			}

			file.close();

			solution.fileName = fileName;
			solution.maximalLength = 0;
			word = "";

			//adauga un spatiu alb la final pentru a procesa cuvantul la ultimul
			//pas al forului
			fragment.add((byte) ' ');

			//proceseaza fragmentul
			for (byte chr : fragment) {
				if (!whitespace(chr))
					word += (char) chr; //construieste cuvant
				else if (word.length() > 0) {
					//proceseaza cuvant
					
					appearances = solution.appearances.get(word.length());
					if (appearances == null) {
						solution.appearances.put(word.length(), 0);
						appearances = 0;
					}

					solution.appearances.put(word.length(), appearances + 1);

					//verifica daca este maximal
					if (word.length() > solution.maximalLength) {
						solution.maximalLength = word.length();
						solution.maximalWords.clear();
						solution.maximalWords.add(word);
					} else if (word.length() == solution.maximalLength) {
						solution.maximalWords.add(word);
					}
					word = "";
				}
			}

			synchronized (solutions) {
				solutions.add(solution);
			}
		} catch (IOException e) {
			e.printStackTrace();
			return;
		}
	}
}