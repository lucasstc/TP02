import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.Locale;
import java.util.Scanner;

public class Q04 {

    static class Data {

        private int ano;
        private int mes;
        private int dia;

        public int getAno() {
            return ano;
        }

        public int getMes() {
            return mes;
        }

        public int getDia() {
            return dia;
        }

        public static Data parseData(String s) {
            String[] partes = s.split("-");

            Data d = new Data();

            d.ano = Integer.parseInt(partes[0]);
            d.mes = Integer.parseInt(partes[1]);
            d.dia = Integer.parseInt(partes[2]);

            return d;
        }

        public String format() {
            return String.format(
                "%02d/%02d/%04d",
                dia,
                mes,
                ano
            );
        }
    }

    static class Veiculo {

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

        public int getId() {
            return id;
        }

        public String getMarca() {
            return marca;
        }

        public String getModelo() {
            return modelo;
        }

        public int getAno() {
            return ano;
        }

        public String getCategoria() {
            return categoria;
        }

        public String[] getCombustivel() {
            return combustivel;
        }

        public int getCilindros() {
            return cilindros;
        }

        public double getCilindrada() {
            return cilindrada;
        }

        public String getTransmissao() {
            return transmissao;
        }

        public String getTracao() {
            return tracao;
        }

        public double getConsumoCidade() {
            return consumoCidade;
        }

        public double getConsumoEstrada() {
            return consumoEstrada;
        }

        public double getCo2() {
            return co2;
        }

        public boolean getTurbo() {
            return turbo;
        }

        public Data getDataRegistro() {
            return dataRegistro;
        }

        public String format() {
            String combs = "";

            for (int i = 0; i < combustivel.length; i++) {
                combs += combustivel[i];

                if (i < combustivel.length - 1) {
                    combs += ",";
                }
            }

            return "[" +
                id + " ## " +
                marca + " ## " +
                modelo + " ## " +
                ano + " ## " +
                categoria + " ## [" +
                combs + "] ## " +
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

        public static Veiculo parseVeiculo(String s) {
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

    static class LeitorCsv {

        public Veiculo[] ler(String caminho) throws IOException {
            BufferedReader br = new BufferedReader(
                new FileReader(caminho)
            );

            Veiculo[] temp = new Veiculo[600];

            int n = 0;

            br.readLine();

            String linha = br.readLine();

            while (linha != null) {
                temp[n] = Veiculo.parseVeiculo(linha);
                n++;

                linha = br.readLine();
            }

            br.close();

            Veiculo[] veiculos = new Veiculo[n];

            for (int i = 0; i < n; i++) {
                veiculos[i] = temp[i];
            }

            return veiculos;
        }
    }

    public static Veiculo pesquisar(
        Veiculo[] v,
        int id
    ) {
        for (int i = 0; i < v.length; i++) {
            if (v[i].getId() == id) {
                return v[i];
            }
        }

        return null;
    }

    public static void insercao(
        Veiculo[] a,
        int n
    ) {
        for (int i = 1; i < n; i++) {
            Veiculo tmp = a[i];

            int j = i - 1;

            while (
                j >= 0 &&
                a[j].getMarca().compareTo(tmp.getMarca()) > 0
            ) {
                a[j + 1] = a[j];
                j--;
            }

            a[j + 1] = tmp;
        }
    }

    public static void main(String[] args) throws IOException {

        Scanner sc = new Scanner(System.in);

        Veiculo[] vet = new LeitorCsv().ler(
            "/tmp/veiculos.csv"
        );

        Veiculo[] a = new Veiculo[1000];

        int n = 0;

        int id = sc.nextInt();

        while (id != -1) {

            Veiculo v = pesquisar(
                vet,
                id
            );

            if (v != null) {
                a[n] = v;
                n++;
            }

            id = sc.nextInt();
        }

        insercao(
            a,
            n
        );

        for (int i = 0; i < n; i++) {
            System.out.println(
                a[i].format()
            );
        }

        sc.close();
    }
 {
 }   
}
