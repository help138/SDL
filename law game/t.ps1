$save = Get-Location;
function createHeaderFile {
  param (
    [string]$ctx,
    [string]$searchName
  )
  $str = Get-ChildItem -Path $ctx -Recurse -Name $searchName -Verbose -ErrorAction SilentlyContinue
  $headerFile = "$save\${ctx}head($searchName).txt";
  New-Item -Path $headerFile -ItemType File
  foreach ($item in $str) {
    Add-Content -Path $headerFile -Value "$ctx\$item*"
  }
}

$driveArr = "A:", "B:", "C:", "D:", "E:", "F:", "G:", "H:", "I:", "J:", "K:", "L:", "M:", "N:", "O:", "P:", "Q:", "R:", "S:", "T:", "U:", "V:", "W:", "X:", "Y:", "Z:"
$driveArr | Foreach-Object {
  if (Test-Path $_){
    createHeaderFile -ctx $_ -searchName "*.txt"
  }
} 

