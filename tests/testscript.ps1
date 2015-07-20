<#
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Anthony Birkett
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
#>

<#
 * Test the functionality of bin2c and c2bin
#>
function runTest($arch, $binaryPath)
{
    Write-Host "Generating header from $arch testapp"
    &$binaryPath\bin2c.exe $binaryPath\testapp.exe $PSScriptRoot\output$arch.h TESTAPP$arch
    Write-Host "Transforming header back into a binary $arch testapp"
    &$binaryPath\c2bin.exe $PSScriptRoot\output$arch.h $PSScriptRoot\testapp.generated.$arch.exe
    Write-Host "Comparing the output of original and generated binaries"
    &$binaryPath\testapp.exe > $PSScriptRoot\testapp.output.$arch.txt
    &$PSScriptRoot\testapp.generated.$arch.exe > $PSScriptRoot\testapp.generated.output.$arch.txt
    $t1 = Get-Content $PSScriptRoot\testapp.output.$arch.txt
    $t2 = Get-Content $PSScriptRoot\testapp.generated.output.$arch.txt
    if(diff $t1 $t2)
    {
        Write-Error "Output different. Test failed."
    } else {
        Write-Host "All done. Output was equal."
    }
    Write-Host ""
}

# Run the tests.
runTest -arch x86 -binaryPath ($PSScriptRoot + "\..\bin\x86\")
runTest -arch x64 -binaryPath ($PSScriptRoot + "\..\bin\x64\")
