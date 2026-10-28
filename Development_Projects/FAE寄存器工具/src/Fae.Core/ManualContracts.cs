namespace Fae.Core;

public record ManualPage(string ImagePath, int PageNumber);
public record ManualField(string Name, int Lsb, int Width, string Description, string Values, string Access, string Reset);
public record ManualRegister(string Name, string Introduction, List<ManualField> Fields, List<ManualPage> Pages, string SourceDocument, string SourceSection, int FirstPage, int LastPage);
public record ManualPack(string Version, Dictionary<string, ManualRegister> Documents, Dictionary<string, string> RegisterDocuments);
