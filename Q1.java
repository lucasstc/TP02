import java.io.*;
import java.util.*;

class Data {

    private int ano;
    private int mes;
    private int dia;

    public Data(int ano, int mes, int dia) {
        this.ano = ano;
        this.mes = mes;
        this.dia = dia;
    }

    public static Data parseData(String s) {

        
        String[] partes = s.split("-");

        int ano = Integer.parseInt(partes[0]);
        int mes = Integer.parseInt(partes[1]);
        int dia = Integer.parseInt(partes[2]);

        return new Data(ano, mes, dia);
    }

    public String format() {
        return String.format("%02d/%02d/%04d", dia, mes, ano);
    }
}


class Veiculo {

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

    public Veiculo(
            int id,
            String marca,
            String modelo,
            int ano,
            String categoria,
            String[] combustivel,
            int cilindros,
            double cilindrada,
            String transmissao,
            String tracao,
            double consumoCidade,
            double consumoEstrada,
            double co2,
            boolean turbo,
            Data dataRegistro) {

        this.id = id;
        this.marca = marca;
        this.modelo = modelo;
        this.ano = ano;
        this.categoria = categoria;
        this.combustivel = combustivel;
        this.cilindros = cilindros;
        this.cilindrada = cilindrada;
        this.transmissao = transmissao;
        this.tracao = tracao;
        this.consumoCidade = consumoCidade;
        this.consumoEstrada = consumoEstrada;
        this.co2 = co2;
        this.turbo = turbo;
        this.dataRegistro = dataRegistro;
    }

    public int getId() {
        return id;
    }


    
    public static Veiculo parseVeiculo(String s) {

        String[] campos = s.split(",");

        int id = Integer.parseInt(campos[0]);

        String marca = campos[1];

        String modelo = campos[2];

        int ano = Integer.parseInt(campos[3]);

        String categoria = campos[4];

        
        String[] combustivel = campos[5].split(";");

        int cilindros = Integer.parseInt(campos[6]);

        double cilindrada = Double.parseDouble(campos[7]);

        String transmissao = campos[8];

        String tracao = campos[9];

        double consumoCidade = Double.parseDouble(campos[10]);

        double consumoEstrada = Double.parseDouble(campos[11]);

        double co2 = Double.parseDouble(campos[12]);

        boolean turbo = Boolean.parseBoolean(campos[13]);

        Data dataRegistro = Data.parseData(campos[14]);

        return new Veiculo(
                id,
                marca,
                modelo,
                ano,
                categoria,
                combustivel,
                cilindros,
                cilindrada,
                transmissao,
                tracao,
                consumoCidade,
                consumoEstrada,
                co2,
                turbo,
                dataRegistro
        );
    }


    public String format() {

        String combustiveis = "[";

        for (int i = 0; i < combustivel.length; i++) {

            combustiveis += combustivel[i];

            if (i < combustivel.length - 1) {
                combustiveis += ",";
            }
        }

        combustiveis += "]";


        return "[" +
                id + " ## " +
                marca + " ## " +
                modelo + " ## " +
                ano + " ## " +
                categoria + " ## " +
                combustiveis + " ## " +
                cilindros + " ## " +
                String.format(Locale.US, "%.1f", cilindrada) + " ## " +
                transmissao + " ## " +
                tracao + " ## " +
                String.format(Locale.US, "%.2f", consumoCidade) + " ## " +
                String.format(Locale.US, "%.2f", consumoEstrada) + " ## " +
                String.format(Locale.US, "%.1f", co2) + " ## " +
                turbo + " ## " +
                dataRegistro.format() +
                "]";
    }
}


class LeitorCsv {

    public static Veiculo[] ler(String caminhoArquivo) {

        ArrayList<Veiculo> lista = new ArrayList<>();

        try {

            BufferedReader br =
                    new BufferedReader(new FileReader(caminhoArquivo));

            
            br.readLine();

            String linha;

            while ((linha = br.readLine()) != null) {

                Veiculo veiculo =
                        Veiculo.parseVeiculo(linha);

                lista.add(veiculo);
            }

            br.close();

        } catch (IOException e) {

            System.out.println("Erro ao ler arquivo.");

        }

        return lista.toArray(new Veiculo[0]);
    }
}


public class Q1 {

   
    public static Veiculo pesquisar(
            Veiculo[] veiculos,
            int id) {

        for (int i = 0; i < veiculos.length; i++) {

            if (veiculos[i].getId() == id) {

                return veiculos[i];

            }
        }

        return null;
    }


    public static void main(String[] args) {

        Locale.setDefault(Locale.US);

        //System.out.println(System.getProperty("user.dir"));

        Scanner scanner = new Scanner(System.in);


        
        Veiculo[] veiculos =
                LeitorCsv.ler("/tmp/veiculos.csv");


        int id = scanner.nextInt();


        while (id != -1) {

            Veiculo encontrado =
                    pesquisar(veiculos, id);


            if (encontrado != null) {

                System.out.println(
                        encontrado.format()
                );

            }


            id = scanner.nextInt();
        }


        scanner.close();
    }
}