#include "pch.h"
#include "Vertex.h"
#include "Mesh.h"


void CMesh::ReleaseUploadBuffers()
{
	if (m_pd3dPositionUploadBuffer) m_pd3dPositionUploadBuffer.Reset();
	if (m_pd3dNormalUploadBuffer) m_pd3dNormalUploadBuffer.Reset();
	if (m_ppd3dIndexUploadBuffers.data())
	{
		m_ppd3dIndexUploadBuffers.clear();
		vector<ComPtr<ID3D12Resource>>().swap(m_ppd3dIndexUploadBuffers);
	}
};

void CMesh::IASetBeforeRender(const ComPtr<ID3D12GraphicsCommandList>& _CommandList)
{
	_CommandList->IASetPrimitiveTopology(m_d3dPrimitiveTopology);
	_CommandList->IASetVertexBuffers(m_nSlot, m_pd3dVertexBufferViews.size(), m_pd3dVertexBufferViews.data());
}

void CMesh::RenderMesh(const ComPtr<ID3D12GraphicsCommandList>& _CommandList, UINT _nSubset)
{
	
	IASetBeforeRender(_CommandList);

	if ((m_nSubMeshes > 0) && (_nSubset < m_nSubMeshes))
	{
		_CommandList->IASetIndexBuffer(&m_pd3dIndexBufferViews[_nSubset]);
		_CommandList->DrawIndexedInstanced(m_pnSubSetIndices[_nSubset], 1, 0, 0, 0);
	}
	else
	{

		_CommandList->DrawInstanced(m_nVertices, 1, m_nOffset, 0);
	}
}

void CMesh::RenderMesh_OneIndexBuffer(const ComPtr<ID3D12GraphicsCommandList>& _CommandList)
{

	IASetBeforeRender(_CommandList);

	if (!m_ppd3dIndexBuffers.empty())
	{
		_CommandList->IASetIndexBuffer(&m_pd3dIndexBufferViews[0]);
		_CommandList->DrawIndexedInstanced(m_nIndices, 1, 0, 0, 0);
	}
	else
	{
		_CommandList->DrawInstanced(m_nVertices, 1, m_nOffset, 0);
	}
}




































//
//CTriangleMesh::CTriangleMesh(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList) : CMesh(_Device, _CommandList)
//{
//	//�ﰢ�� �޽��� �����Ѵ�. 
//	m_nVertices = 3;
//	m_nStride = sizeof(CDiffusedVertex);
//	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
//	/*����(�ﰢ���� ������)�� ������ �ð���� ������� ������, ���, �Ķ������� �����Ѵ�. RGBA(Red, Green, Blue,
//	Alpha) 4���� �Ķ���͸� ����Ͽ� ������ ǥ���Ѵ�. �� �Ķ���ʹ� 0.0~1.0 ������ �Ǽ����� ������.*/
//	CDiffusedVertex pVertices[3];
//	pVertices[0] = CDiffusedVertex(XMFLOAT3(0.0f, 0.5f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f,
//		1.0f));
//	pVertices[1] = CDiffusedVertex(XMFLOAT3(0.5f, -0.5f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f,
//		1.0f));
//	pVertices[2] = CDiffusedVertex(XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT4(Colors::Blue));
//	//�ﰢ�� �޽��� ���ҽ�(���� ����)�� �����Ѵ�. 
//	m_pd3dVertexBuffer = Util::CreateBufferResource(_Device, _CommandList, pVertices, m_nStride* m_nVertices, D3D12_HEAP_TYPE_DEFAULT,
//		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, & m_pd3dVertexUploadBuffer);
//		//���� ���� �並 �����Ѵ�. 
//		m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
//		m_d3dVertexBufferView.StrideInBytes = m_nStride; //������ ũ�� ����Ʈ�� color�����̶� XMFLOAT4 ->16����Ʈ 
//		m_d3dVertexBufferView.SizeInBytes = m_nStride * m_nVertices; //���� ���� ��ü ũ��
//}
//
//
//
//CCubeMeshDiffused::CCubeMeshDiffused(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList, float fWidth, float fHeight, float fDepth) : CMesh(_Device,
//	_CommandList)
//{
//	//������ü�� ������(����)�� 8���̴�.
//	m_nVertices = 8;
//	m_nStride = sizeof(CDiffusedVertex);
//	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
//	float fx = fWidth * 0.5f, fy = fHeight * 0.5f, fz = fDepth * 0.5f;
//	//���� ���۴� ������ü�� ������ 8���� ���� ���� �����͸� ������.
//	CDiffusedVertex pVertices[8];
//	pVertices[0] = CDiffusedVertex(XMFLOAT3(-fx, +fy, -fz), RANDOM_COLOR);
//	pVertices[1] = CDiffusedVertex(XMFLOAT3(+fx, +fy, -fz), RANDOM_COLOR);
//	pVertices[2] = CDiffusedVertex(XMFLOAT3(+fx, +fy, +fz), RANDOM_COLOR);
//	pVertices[3] = CDiffusedVertex(XMFLOAT3(-fx, +fy, +fz), RANDOM_COLOR);
//	pVertices[4] = CDiffusedVertex(XMFLOAT3(-fx, -fy, -fz), RANDOM_COLOR);
//	pVertices[5] = CDiffusedVertex(XMFLOAT3(+fx, -fy, -fz), RANDOM_COLOR);
//	pVertices[6] = CDiffusedVertex(XMFLOAT3(+fx, -fy, +fz), RANDOM_COLOR);
//	pVertices[7] = CDiffusedVertex(XMFLOAT3(-fx, -fy, +fz), RANDOM_COLOR);
//	m_pd3dVertexBuffer = Util::CreateBufferResource(_Device, _CommandList, pVertices,
//		m_nStride * m_nVertices, D3D12_HEAP_TYPE_DEFAULT,
//		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dVertexUploadBuffer);
//	m_d3dVertexBufferView.BufferLocation = m_pd3dVertexBuffer->GetGPUVirtualAddress();
//	m_d3dVertexBufferView.StrideInBytes = m_nStride;
//	m_d3dVertexBufferView.SizeInBytes = m_nStride * m_nVertices;
//	/*�ε��� ���۴� ������ü�� 6���� ��(�簢��)�� ���� ���� ������ ���´�. �ﰢ�� ����Ʈ�� ������ü�� ǥ���� ����
//	�Ƿ� �� ���� 2���� �ﰢ���� ������ �� �ﰢ���� 3���� ������ �ʿ��ϴ�. ��, �ε��� ���۴� ��ü 36(=6*2*3)���� ��
//	������ ������ �Ѵ�.*/
//	m_nIndices = 36;
//	UINT pnIndices[36];
//	//�� �ո�(Front) �簢���� ���� �ﰢ��
//	pnIndices[0] = 3; pnIndices[1] = 1; pnIndices[2] = 0;
//	//�� �ո�(Front) �簢���� �Ʒ��� �ﰢ��
//	pnIndices[3] = 2; pnIndices[4] = 1; pnIndices[5] = 3;
//	//�� ����(Top) �簢���� ���� �ﰢ��
//	pnIndices[6] = 0; pnIndices[7] = 5; pnIndices[8] = 4;
//	//�� ����(Top) �簢���� �Ʒ��� �ﰢ��
//	pnIndices[9] = 1; pnIndices[10] = 5; pnIndices[11] = 0;
//	//�� �޸�(Back) �簢���� ���� �ﰢ��
//	pnIndices[12] = 3; pnIndices[13] = 4; pnIndices[14] = 7;
//	//�� �޸�(Back) �簢���� �Ʒ��� �ﰢ��
//	pnIndices[15] = 0; pnIndices[16] = 4; pnIndices[17] = 3;
//	//�� �Ʒ���(Bottom) �簢���� ���� �ﰢ��
//	pnIndices[18] = 1; pnIndices[19] = 6; pnIndices[20] = 5;
//	//�� �Ʒ���(Bottom) �簢���� �Ʒ��� �ﰢ��
//	pnIndices[21] = 2; pnIndices[22] = 6; pnIndices[23] = 1;
//	//�� ����(Left) �簢���� ���� �ﰢ��
//	pnIndices[24] = 2; pnIndices[25] = 7; pnIndices[26] = 6;
//	//�� ����(Left) �簢���� �Ʒ��� �ﰢ��
//	pnIndices[27] = 3; pnIndices[28] = 7; pnIndices[29] = 2;
//	//�� ����(Right) �簢���� ���� �ﰢ��
//	pnIndices[30] = 6; pnIndices[31] = 4; pnIndices[32] = 5;
//	//�� ����(Right) �簢���� �Ʒ��� �ﰢ��
//	pnIndices[33] = 7; pnIndices[34] = 4; pnIndices[35] = 6;
//	//�ε��� ���۸� �����Ѵ�.
//	m_pd3dIndexBuffer = Util::CreateBufferResource(_Device, _CommandList, pnIndices,
//	sizeof(UINT)* m_nIndices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER,
//		& m_pd3dIndexUploadBuffer);
//		//�ε��� ���� �並 �����Ѵ�. 
//	m_d3dIndexBufferView.BufferLocation = m_pd3dIndexBuffer->GetGPUVirtualAddress();
//		m_d3dIndexBufferView.Format = DXGI_FORMAT_R32_UINT;
//		m_d3dIndexBufferView.SizeInBytes = sizeof(UINT) * m_nIndices;
//}
//CCubeMeshDiffused::~CCubeMeshDiffused()
//{
//}