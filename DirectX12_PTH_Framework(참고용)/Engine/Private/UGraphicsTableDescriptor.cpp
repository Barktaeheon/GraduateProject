#include "EngineDefines.h"
#include "UGraphicsTableDescriptor.h"
#include "UDevice.h"
#include "UGpuCommand.h"

UGraphicsTableDescriptor::UGraphicsTableDescriptor() : UTableDescriptor()
{
}

void UGraphicsTableDescriptor::Free()
{

}

void UGraphicsTableDescriptor::CommitTable(CSHPTRREF<UCommand> _spCommand)
{
	// CommitTable ->DescriptorHeapStart�� �����ͼ� Group��ŭ Bind �ϴ� ���̴�. 
	_spCommand->GetGpuCmdList()->SetGraphicsRootDescriptorTable(GetStaticValue(), ReturnCommitTableHandle(_spCommand));
}