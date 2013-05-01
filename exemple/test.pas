program ProgTest;

var
	FirstVarGlobal : integer;
	SecondVarGlobal, ThirdVarGlobal : boolean;
	Tab : array [42..73] of integer;

procedure proc (FirstParamProc : integer; SecondParamProc : boolean);
var
	VarLocalProc : boolean;
begin
	FirstVarGlobal := FirstVarGlobal div FirstParamProc;
end;

var
	QuatrVarGlobal : integer;

function Fonc (ParamFonction : integer; TabParamFonc : array [42..73] of integer) : integer;
begin
	Fonc := ParamFonction * TabParamFonc [50];
end;

begin
	write(Fonc(2, Tab));
	if FirstVarGlobal > 3 then
		proc(FirstVarGlobal, false);
	
	Fonc (5, Fonc(5, Fonc (4, 5)));
	Fonc(2, Tab);
	
	proc(5, true);
end.
