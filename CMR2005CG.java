    import java.util.Arrays;
    
    public class Main
    {

        public static String[] cheats = { "All tracks code", "4WD cars code", "2WD cars code", "Super 2WD cars code", "RWD cars code", "4x4 cars code", "Classic cars code", "Special cars code", "Group B cars code", "Mirror mode code (inaccessible)" };

        private static boolean Contains(String[] array, String text)
        {
            return Arrays.asList(array).indexOf(text) != -1;
        }

        private static boolean IsValidCode(String text)
        {
            try
            {
                long n = Long.parseLong(text, 10);
                return true;
            }
            catch (Exception ex)
            {
                return false;
            }
        }

        public static void main(String[] args)
        {
            System.out.println("Colin McRae Rally 2005 Cheat Generator");
            if (args.length != 4 || (!Contains(args, "-p") && !Contains(args, "-P")) || (!Contains(args, "-c") && !Contains(args, "-C")))
            {
                System.out.println("Usage: CMR2005CG [commands]");
                System.out.println("Commands  Parameters            Explanation");
                System.out.println("-p        [pc, ps2, psp, xbox]  Set game platform");
                System.out.println("-c        [1, ..., 99999]       Set access code");
                return;
            }
            int ip = Contains(args, "-p") ? Arrays.asList(args).indexOf("-p") : Arrays.asList(args).indexOf("-P");
            int ic = Contains(args, "-c") ? Arrays.asList(args).indexOf("-c") : Arrays.asList(args).indexOf("-C");
            if (!IsValidCode(args[ic + 1]))
            {
                System.out.println("Invalid access code");
                return;
            }
            String platform = args[ip + 1].toLowerCase();
            long accessCode = Long.parseLong(args[ic + 1]);
            if (accessCode < 1L || accessCode > 99999L)
            {
                System.out.println("Invalid access code. Valid access codes are in range 1 - 99999");
                return;
            }
            long[] array1;
            long[] array2;
            switch (platform)
            {
                case "pc":
                {
                        array1 = new long[] { 331L, 1259L, 241L, 109L, 521L, 853L, 71L, 719L, 941L, 269L };
                        array2 = new long[] { 72481L, 180307L, 130241L, 392827L, 421019L, 949147L, 32801L, 1296649L, 91249L, 639679L };
                        break;
                }

                case "ps2":
                case "xbox":
                {
                        array1 = new long[] { 859L, 773L, 151L, 47L, 487L, 211L, 617L, 131L, 947L, 313L };
                        array2 = new long[] { 69119L, 67783L, 70271L, 77929L, 238099L, 148151L, 472751L, 818963L, 1195489L, 839381L };
                        break;
                }

                case "psp":
                {
                        array1 = new long[] { 743L, 1663L, 227L, 991L, 443L, 89L, 571L, 199L, 1373L, 601L };
                        array2 = new long[] { 35491L, 783019L, 1116491L, 591319L, 194591L, 37369L, 822839L, 86083L, 354661L, 99809L };
                        break;
                }

                default:
                {
                        System.out.println("Invalid platform. Enter a platform among pc, ps2, psp, xbox");
                        return;
                }
            }
            System.out.println("Platform: " + platform.toUpperCase());
            System.out.println("Access code: " + accessCode);
            for (int i = 0; i < cheats.length; i++)
            {
                System.out.println(cheats[i] + ": " + GenerateCode(array1, array2, accessCode, i));
            }
        }

        private static String GenerateCode(long[] array1, long[] array2, long accessCode, long cheatID)
        {
            long cheatIDMagic = 0x13CB5BL * cheatID % 0x26DDL;
            long accessCodeMagic = (accessCode % 0x3E8L) ^ (0x20B9L * cheatIDMagic);
            long seed1 = CalculateSeed(accessCodeMagic % 0x9ADL);
            long seed2 = CalculateSeed(Remainder(ToSigned32((accessCodeMagic ^ 0x114CF1L) * ((0x41BL * cheatIDMagic) ^ Remainder(Quotient(accessCode, 0x3E8L), 0x3E8L))), 0x91DL));
            long[] buffer = { 0L, 0L, 0L, 0L, 0L, 0L };
            buffer[0] = Remainder(seed1, 26L) + 65L;
            buffer[1] = Remainder(Quotient(seed1, 676L), 26L) + 65L;
            buffer[2] = Remainder(Quotient(seed1, 26L), 26L) + 65L;
            buffer[3] = Remainder(Quotient(seed2, 26L), 26L) + 65L;
            buffer[4] = Remainder(Quotient(seed2, 676L), 26L) + 65L;
            buffer[5] = Remainder(seed2, 26L) + 65L;
            long bufMidXor = CalculateFeedback(buffer);
            long feedback1 = ToSigned32((buffer[0] << 24) + (buffer[1] << 16) + (buffer[2] << 8) + buffer[3]);
            long feedback2 = ToSigned32((buffer[4] << 24) + (buffer[5] << 16) + ((bufMidXor + Remainder(cheatIDMagic ^ 0x197ABD9L, seed1 & 0xFFFFFFFFL)) << 8) + bufMidXor + Remainder(cheatIDMagic ^ 0x13478FDDL, seed2 & 0xFFFFFFFFL));
            long tmpFeedback2 = 0L;
            for (int i = 0; i < 42; i++)
            {
                tmpFeedback2 = feedback2;
                feedback2 = feedback1 ^ array1[i % 10];
                feedback1 = tmpFeedback2 ^ feedback1;
            }
            for (int i = 0; i < 277; i++)
            {
                tmpFeedback2 = feedback2;
                feedback2 = feedback1 ^ array2[i % 10];
                feedback1 = feedback1 ^ tmpFeedback2;
            }
            buffer[0] = Remainder((feedback2 >> 24) & 0xFFL, 26L) + 65L;
            buffer[1] = Remainder((feedback2 >> 16) & 0xFFL, 26L) + 65L;
            buffer[2] = Remainder((feedback1 >> 24) & 0xFFL, 26L) + 65L;
            buffer[3] = Remainder((feedback1 >> 16) & 0xFFL, 26L) + 65L;
            buffer[4] = Remainder((feedback1 >> 8) & 0xFFL, 26L) + 65L;
            buffer[5] = Remainder(feedback1 & 0xFFL, 26L) + 65L;
            String code = "";
            for (int i = 0; i < 6; i++)
            {
                code += (char) buffer[i];
            }
            return code;
        }

        private static long Quotient(long n, long m)
        {
            return n / m;
        }

        private static long Remainder(long n, long m)
        {
            return n % m;
        }

        private static long CalculateFeedback(long[] buffer)
        {
            long result = 0;
            for (int i = 0; i < buffer.length - 1; i++)
            {
                result += buffer[i] ^ 0x13C501L;
            }
            return ToSigned32(result);
        }

        private static long ToSigned32(long n)
        {
            return ((n & 0xFFFFFFFFL) ^ 0x80000000L) - 0x80000000L;
        }

        private static long CalculateSeed(long n)
        {
            long seed = 1L;
            if (n != 0L)
            {
                seed = 0xF82DL;
                for (int i = 0; i < n - 1; i++)
                {
                    seed = Remainder(ToSigned32(0xF82DL * seed), 0x5243L);
                }
            }
            return seed;
        }
    }
