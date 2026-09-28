import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Scanner;

public class Q09
{
	static class Data
	{
		//atributos
		private int ano;
		private int mes;
		private int dia;

		//gets
		public int getAno() 
		{
			return ano;
		}
		public int getMes() 
		{
			return mes;	
		}
		public int getDia() 
		{
			return dia;
		}
		public static Data parseData(String s) //converte uma string no formato aaaa-mm-dd em um objeto Data
		{
        	String[] partes = s.split("-");          
        	Data d = new Data();
        	d.ano = Integer.parseInt(partes[0]);
        	d.mes = Integer.parseInt(partes[1]);
        	d.dia = Integer.parseInt(partes[2]);
        return d;
    	}
    	public String format() //formata a data no padrão dd/mm/aaaa
		{
        	return String.format("%02d/%02d/%04d", dia, mes, ano);
    	}
	}
	static class Veiculo
	{
		//atributos
		private int id;
		private String marca;
		private String modelo;
		private int ano;
		private String categoria;
		private String[] combustivel;
		private int cilindros;
		private double cilindrada;
		private String transmissao;
		private String tracao;
		private double consumoCidade;
		private double consumoEstrada;
		private double co2;
		private boolean turbo;
		private Data dataRegistro;

		//gets
		public int getId() 
		{
    		return id;
		}
		public String getMarca() 
		{
    		return marca;
		}
		public String getModelo() 
		{
    		return modelo;
		}
		public int getAno() 
		{
			return ano;
		}
		public String getCategoria() 
		{
			return categoria;
		}
		public String[] getCombustivel() 
		{
			return combustivel;
		}
		public int getCilindros() 
		{
			return cilindros;
		}
		public double getCilindrada() 
		{
			return cilindrada;
		}
		public String getTransmissao() 
		{
			return transmissao;
		}
		public String getTracao() 
		{
			return tracao;
		}
		public double getConsumoCidade() 
		{
			return consumoCidade;
		}
		public double getConsumoEstrada() 
		{
			return consumoEstrada;
		}
		public double getCo2() 
		{
			return co2;
		}	
		public boolean getTurbo() 
		{ 
    		return turbo;
		}
		public Data getDataRegistro() 
		{
    		return dataRegistro;
		}
		
		public String format() //formata os dados do veículo em uma string para exibição na saída
		{
			String combs = "";
    		for (int i = 0; i < combustivel.length; i++) 
			{
        		combs = combs + combustivel[i];
        		if (i < combustivel.length - 1) 
				{
            		combs = combs + ",";
        		}
    		}
    		return "[" + id + " ## " + marca + " ## " + modelo + " ## " + ano + " ## " + categoria + " ## [" + combs + "] ## " + cilindros + " ## " + String.format(java.util.Locale.US, "%.1f", cilindrada) + " ## " + transmissao + " ## " + tracao + " ## " + String.format(java.util.Locale.US, "%.2f", consumoCidade) + " ## " + String.format(java.util.Locale.US, "%.2f", consumoEstrada) + " ## " + String.format(java.util.Locale.US, "%.1f", co2) + " ## " + turbo + " ## " + dataRegistro.format() + "]";
		}
		public static Veiculo parseVeiculo(String s) //converte uma linha do arquivo CSV em um objeto Veiculo
		{
			String[] partes = s.split(",");
			Veiculo v = new Veiculo();
			v.id = Integer.parseInt(partes[0]);
			v.marca = partes[1];
			v.modelo = partes[2];
			v.ano = Integer.parseInt(partes[3]);
			v.categoria = partes[4];
			v.combustivel = partes[5].split(";");
			v.cilindros = Integer.parseInt(partes[6]);
			v.cilindrada = Double.parseDouble(partes[7]);
			v.transmissao = partes[8];
			v.tracao = partes[9];
			v.consumoCidade = Double.parseDouble(partes[10]);
			v.consumoEstrada = Double.parseDouble(partes[11]);
			v.co2 = Double.parseDouble(partes[12]);
			v.turbo = Boolean.parseBoolean(partes[13]);
			v.dataRegistro = Data.parseData(partes[14]);
			return v;
		}
	}
	static class LeitorCsv //classe para ler o arquivo CSV e criar um array (utilizado para pesquisa) de objetos Veiculo
	{  
		public Veiculo[] ler(String caminho) throws IOException 
		{
			BufferedReader br = new BufferedReader(new FileReader(caminho));
			Veiculo[] temp = new Veiculo[600];
			int n = 0;
			br.readLine();                    // descarta o cabeçalho
			String linha = br.readLine();
			while (linha != null) 
			{
				temp[n] = Veiculo.parseVeiculo(linha);
				n = n+1;
				linha = br.readLine();
			}
			br.close();
			Veiculo[] veiculos = new Veiculo[n];   // array do tamanho exato
			for (int i = 0; i < n; i++) 
			{
				veiculos[i] = temp[i];
			}
			return veiculos;
		}
	}

	public static Veiculo pesquisar(Veiculo[] v, int id) //pesquisa sequencial
	{
		for (int i = 0; i < v.length; i++) 
		{
			if (v[i].getId() == id) 
			{
				return v[i];
			}
		}
		return null;
	}

	// Lista com alocação sequencial
	static class ListaSeq
	{
		private Veiculo[] array;
		private int n;

		public ListaSeq()
		{
			this(1000);
		}

		public ListaSeq(int tamanho)
		{
			array = new Veiculo[tamanho];
			n = 0;
		}

		public void inserirInicio(Veiculo veiculo) throws Exception
		{
			if (n >= array.length)
			{
				throw new Exception("Erro!"); 
			}
			for (int i = n; i > 0; i--) 
			{
				array[i] = array[i - 1]; //desloca elementos para a direita
			}
			array[0] = veiculo; //insere no início
			n++;
		}

		public void inserirFim(Veiculo veiculo) throws Exception
		{
			if (n >= array.length)
			{
				throw new Exception("Erro!");
			}
			array[n] = veiculo; //insere no final
			n++;
		}

		public void inserir(Veiculo veiculo, int posicao) throws Exception
		{
			if (n >= array.length || posicao < 0 || posicao > n)
			{
				throw new Exception("Erro!");
			}
			for (int i = n; i > posicao; i--)
			{
				array[i] = array[i - 1]; //desloca elementos para a direita
			}
			array[posicao] = veiculo; //insere na posição desejada
			n++;
		}

		public Veiculo removerInicio() throws Exception
		{
			if (n == 0)
			{
				throw new Exception("Erro!");
			}
			Veiculo resp = array[0];
			n--;
			for (int i = 0; i < n; i++)
			{
				array[i] = array[i + 1]; //desloca elementos para a esquerda
			}
			return resp; //retorna o elemento removido
		}

		public Veiculo removerFim() throws Exception
		{
			if (n == 0)
			{
				throw new Exception("Erro!");
			}
			n--; //retira 1 do tamanho da lista
			return array[n]; //retorna o elemento removido
		}

		public Veiculo remover(int posicao) throws Exception
		{
			if (n == 0 || posicao < 0 || posicao >= n)
			{
				throw new Exception("Erro!");
			}
			Veiculo resp = array[posicao]; //guarda o elemento a ser removido
			n--; //retira 1 do tamanho da lista
			for (int i = posicao; i < n; i++)
			{
				array[i] = array[i + 1]; //desloca elementos para a esquerda
			}
			return resp; //retorna o elemento removido
		}

		public void mostrar()
		{
			for (int i = 0; i < n; i++)
			{
				System.out.println(array[i].format()); //exibe os elementos da lista
			}
		}
	}

	public static void main(String[] args) throws Exception
	{
		Scanner sc = new Scanner(System.in);
		Veiculo[] vet = new LeitorCsv().ler("/tmp/veiculos.csv");
		ListaSeq lista = new ListaSeq();

		// primeira parte: ids inseridos no fim da lista
		int id = sc.nextInt();
		while (id != -1)
		{
			Veiculo v = pesquisar(vet, id);
			if (v != null)
			{
				lista.inserirFim(v);
			}
			id = sc.nextInt();
		}

		// segunda parte: comandos
		int qtd = sc.nextInt();
		for (int i = 0; i < qtd; i++)
		{
			String cmd = sc.next();
			if (cmd.equals("II"))
			{
				lista.inserirInicio(pesquisar(vet, sc.nextInt()));
			}
			else if (cmd.equals("IF"))
			{
				lista.inserirFim(pesquisar(vet, sc.nextInt()));
			}
			else if (cmd.equals("I*"))
			{
				int pos = sc.nextInt();
				lista.inserir(pesquisar(vet, sc.nextInt()), pos);
			}
			else
			{
				Veiculo r;
				if (cmd.equals("RI"))
				{
					r = lista.removerInicio();
				}
				else if (cmd.equals("RF"))
				{
					r = lista.removerFim();
				}
				else
				{
					r = lista.remover(sc.nextInt());
				}
				System.out.println("(R)" + r.getMarca() + " " + r.getModelo());
			}
		}

		lista.mostrar();
		sc.close();
	}
}
