program ProgTest;

var
FirstVarGlobal : integer;
SecondVarGlobal, ThirdVarGlobal : boolean;

procedure proc (FirstParamProc : integer; SecondParamProc : boolean);
var
VarLocalProc : integer;
begin
FirstVarGlobal := VarLocalProc div FirstParamProc;
end;

var
QuatrVarGlobal : integer;

function Fonc (ParamFonction : integer; TabParamFonc : array [42..73] of boolean) : integer;
begin
Fonc := ParamFonction * ThirdVarGlobal;
end;

begin
write(Fonc(2));
end.
