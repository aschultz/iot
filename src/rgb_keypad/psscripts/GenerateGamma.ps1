
function GenerateGamma()
{
	param(
		[float]$gamma = 2.8,
		[int]$inputBitDepth = 8,
		[int]$outputBitDepth = 8
	)

	$maxIn = (1 -shl $inputBitDepth) - 1
	$maxOut = (1 -shl $outputBitDepth) - 1

	if($outputBitDepth -le 8) { $outputType = "uint8_t" }
	elseif($outputBitDepth -le 16){ $outputType = "uint16_t" }

	$str = "const static uint8_t gamma[$($maxIn+1)] PROGMEM = {"

	for($i=0; $i -le $maxIn; ++$i)
	{
		$result = [int][Math]::Floor([Math]::Pow($i / [float]$maxIn, $gamma) * $maxOut + 0.5)

		if($i -gt 0)
		{
			$str += ","
		}
		$str += "$result"
	}

	$str += "};"

	Write-Output $str
}

GenerateGamma -gamma 4 -inputBitDepth 8 -outputBitDepth 16