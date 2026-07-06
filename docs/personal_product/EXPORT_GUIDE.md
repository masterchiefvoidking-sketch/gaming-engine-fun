# Export Guide

## Profiles

| Profile | Output | Use |
|---------|--------|-----|
| Windows Development | `Build/windows_development/` | Local testing with debug overlay |
| Windows Release | `Build/windows_release/` | Optimized offline build |
| Web Development | `Build/web_development/web/` | Browser testing with touch controls |
| Web Release | `Build/web_release/web/` | Compressed production web build |

## From Editor

```cpp
studio.export_game(ExportTarget::Windows);
studio.export_game(ExportTarget::Web);
```

## From CLI

```bash
./build/apps/personal_product_cut/eve-personal-product-cut
```

## RC1 Bundle

Release artifacts are staged under `Releases/ApartmentLifeDemo_RC1/`:

- `WindowsDev/`, `WindowsRelease/`, `WebDev/`, `WebRelease/`
- `TestReport.md`, `ValidationReport.md`, `PerformanceReport.md`
- `KnownIssues.md`, `ExportInstructions.md`
- `BackupArchive/` (project snapshot)

## Validation

All exports pass the validation gate before cooking. Check `ValidationReport.md` for details.
