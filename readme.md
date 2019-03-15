# Benchmark
Benchmark for various UTF-8 libraries in vcpkg.

Results for Intel(R) Xeon(R) CPU E3-1230 v6 @ 3.50GHz:

benchmark | utfcpp   | utfh     | utf8proc | utfz    |
----------|---------:|---------:|---------:|---------:
validate  |   113 ns |    41 ns |    64 ns |  224 ns |
iterate   |   289 ns |    86 ns |    62 ns |  231 ns |
convert   |   238 ns |   180 ns |   146 ns |  292 ns |

Tested with custom LLVM build and vcpkg toolchain:<br/>
<https://github.com/qis/windows/blob/master/vcpkg.md>
