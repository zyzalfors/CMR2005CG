Imports System.Text.RegularExpressions

Module CMR2005CG

  Function Quotient(n As Long, m As Long) As Long
    Return If(n / m > 0, Math.Floor(n / m), Math.Ceiling(n / m))
  End Function

  Function Remainder(n As Long, m As Long) As Long
    Return n - Quotient(n, m) * m
  End Function

  ReadOnly Cheats As String() = {"All tracks code", "4WD cars code", "2WD cars code", "Super 2WD cars code", "RWD cars code", "4x4 cars code", "Classic cars code", "Special cars code", "Group B cars code", "Mirror mode code (inaccessible)"}

  Function IsValidCode(text As String) As Boolean
    If Regex.IsMatch(text, "[^0-9]") Then Return False
    Try
      Dim n As Long = CType(text, Long)
      Return True
    Catch
      Return False
    End Try
  End Function

  Sub Main(Args As String())
    Console.WriteLine("Colin McRae Rally 2005 Cheat Generator")
    If Args.Length <> 4 OrElse (Not Args.Contains("-p") AndAlso Not Args.Contains("-P")) OrElse (Not Args.Contains("-c") AndAlso Not Args.Contains("-C")) Then
      Console.WriteLine("Usage: CMR2005CG [commands]")
      Console.WriteLine("Commands  Parameters            Explanation")
      Console.WriteLine("-p        [pc, ps2, psp, xbox]  Set game platform")
      Console.WriteLine("-c        [1, ..., 99999]       Set access code")
      Return
    End If
    Dim ip As Int32 = If(Args.Contains("-p"), Array.IndexOf(Args, "-p"), Array.IndexOf(Args, "-P"))
    Dim ic As Int32 = If(Args.Contains("-c"), Array.IndexOf(Args, "-c"), Array.IndexOf(Args, "-C"))
    If Not IsValidCode(Args(ic + 1)) Then
      Console.WriteLine("Invalid access code")
      Return
    End If
    Dim Platform As String = Args(ip + 1).ToLower()
    Dim AccessCode As Long = CType(Args(ic + 1), Long)
    If AccessCode < 1L Or AccessCode > 99999L Then
      Console.WriteLine("Invalid access code. Valid access codes are in range 1 - 99999")
      Return
    End If
    Dim Array1 As Long()
    Dim Array2 As Long()
    Select Case Platform
      Case "pc"
        Array1 = {331L, 1259L, 241L, 109L, 521L, 853L, 71L, 719L, 941L, 269L}
        Array2 = {72481L, 180307L, 130241L, 392827L, 421019L, 49147L, 32801L, 1296649L, 91249L, 639679L}
      Case "ps2", "xbox"
        Array1 = {859L, 773L, 151L, 47L, 487L, 211L, 617L, 131L, 947L, 313L}
        Array2 = {69119L, 67783L, 70271L, 77929L, 238099L, 148151L, 472751L, 818963L, 1195489L, 839381L}
      Case "psp"
        Array1 = {743L, 1663L, 227L, 991L, 443L, 89L, 571L, 199L, 1373L, 601L}
        Array2 = {35491L, 783019L, 1116491L, 591319L, 194591L, 37369L, 822839L, 86083L, 354661L, 99809L}
      Case Else
        Console.WriteLine("Invalid platform. Enter a platform among pc, ps2, psp, xbox")
        Return
    End Select
    Console.WriteLine("Platform: " + Platform.ToUpper())
    Console.WriteLine("Access code: " + AccessCode.ToString())
    For i As Int32 = 0 To Cheats.Length - 1
      Console.WriteLine(Cheats(i) + ": " + GenerateCode(Array1, Array2, AccessCode, i))
    Next i
  End Sub

  Function GenerateCode(Array1 As Long(), Array2 As Long(), AccessCode As Long, CheatID As Long) As String
    Dim CheatIDMagic As Long = &H13CB5BL * CheatID Mod &H26DDL
    Dim AccessCodeMagic As Long = (AccessCode Mod &H3E8L) Xor (&H20B9L * CheatIDMagic)
    Dim Seed1 As Long = CalculateSeed(AccessCodeMagic Mod &H9ADL)
    Dim Seed2 As Long = CalculateSeed(Remainder(ToSigned32((AccessCodeMagic Xor &H114CF1L) * ((&H41BL * CheatIDMagic) Xor Remainder(Quotient(AccessCode, &H3E8L), &H3E8L))), &H91DL))
    Dim Buffer As Long() = {0L, 0L, 0L, 0L, 0L, 0L}
    Buffer(0) = Remainder(Seed1, 26L) + 65L
    Buffer(1) = Remainder(Quotient(Seed1, 676L), 26L) + 65L
    Buffer(2) = Remainder(Quotient(Seed1, 26L), 26L) + 65L
    Buffer(3) = Remainder(Quotient(Seed2, 26L), 26L) + 65L
    Buffer(4) = Remainder(Quotient(Seed2, 676L), 26L) + 65L
    Buffer(5) = Remainder(Seed2, 26L) + 65L
    Dim BufMidXor As Long = CalculateFeedback(Buffer)
    Dim Feedback1 As Long = ToSigned32((Buffer(0) << 24L) + (Buffer(1) << 16L) + (Buffer(2) << 8L) + Buffer(3))
    Dim Feedback2 As Long = ToSigned32((Buffer(4) << 24L) + (Buffer(5) << 16L) + ((BufMidXor + Remainder(CheatIDMagic Xor &H197ABD9L, Seed1 And &HFFFFFFFFL)) << 8L) + BufMidXor + Remainder(CheatIDMagic Xor &H13478FDDL, Seed2 And &HFFFFFFFFL))
    Dim TmpFeedback2 As Long = 0L
    For i As Int32 = 0 To 41
      TmpFeedback2 = Feedback2
      Feedback2 = Feedback1 Xor Array1(i Mod 10)
      Feedback1 = TmpFeedback2 Xor Feedback1
    Next i
    For i As Int32 = 0 To 276
      TmpFeedback2 = Feedback2
      Feedback2 = Feedback1 Xor Array2(i Mod 10)
      Feedback1 = Feedback1 Xor TmpFeedback2
    Next i
    Buffer(0) = Remainder((Feedback2 >> 24L) And &HFFL, 26L) + 65L
    Buffer(1) = Remainder((Feedback2 >> 16L) And &HFFL, 26L) + 65L
    Buffer(2) = Remainder((Feedback1 >> 24L) And &HFFL, 26L) + 65L
    Buffer(3) = Remainder((Feedback1 >> 16L) And &HFFL, 26L) + 65L
    Buffer(4) = Remainder((Feedback1 >> 8L) And &HFFL, 26L) + 65L
    Buffer(5) = Remainder(Feedback1 And &HFFL, 26L) + 65L
    Dim code As String = ""
    For i As Int32 = 0 To 5
      code += Chr(Buffer(i))
    Next i
    Return code
  End Function

  Function CalculateFeedback(Buffer As Long()) As Long
    Dim Result As Long = 0
    For i As Int32 = 0 To Buffer.Length - 2
      Result += Buffer(i) Xor &H13C501L
    Next i
    Return ToSigned32(Result)
  End Function

  Function ToSigned32(n As Long) As Long
    Return ((n And &HFFFFFFFFL) Xor &H80000000L) - &H80000000L
  End Function

  Function CalculateSeed(n As Long) As Long
    Dim Seed As Long = 1L
    If n <> 0L Then
      Seed = &HF82DL
      For i As Int32 = 0 To n - 2
        Seed = Remainder(ToSigned32(&HF82DL * Seed), &H5243L)
      Next i
    End If
    Return Seed
  End Function

End Module

