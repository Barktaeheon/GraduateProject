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

XMFLOAT3 CMesh::TransformPoint(const XMFLOAT3& point, const XMFLOAT4X4& worldMatrix) const
{
	XMVECTOR pointVec = XMLoadFloat3(&point);
	XMMATRIX worldMatrixXM = XMLoadFloat4x4(&worldMatrix);
	XMVECTOR worldtransformedpointVec = XMVector3TransformCoord(pointVec, worldMatrixXM);
	XMFLOAT3 worldtransformedpointf3;
	XMStoreFloat3(&worldtransformedpointf3, worldtransformedpointVec);

	return worldtransformedpointf3;
}

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

CTriangleMesh::CTriangleMesh(const ComPtr<ID3D12Device>& _Device, const ComPtr<ID3D12GraphicsCommandList>& _CommandList) : CMesh(_Device, _CommandList)
{
	//�ﰢ�� �޽��� �����Ѵ�. 
	m_nVertices = 3;
	m_nStride = sizeof(CDiffusedVertex);
	m_d3dPrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	/*����(�ﰢ���� ������)�� ������ �ð���� ������� ������, ���, �Ķ������� �����Ѵ�. RGBA(Red, Green, Blue,
	Alpha) 4���� �Ķ���͸� ����Ͽ� ������ ǥ���Ѵ�. �� �Ķ���ʹ� 0.0~1.0 ������ �Ǽ����� ������.*/
	CDiffusedVertex pVertices[3];
	pVertices[0] = CDiffusedVertex(XMFLOAT3(0.0f, 0.5f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f,
		1.0f));
	pVertices[1] = CDiffusedVertex(XMFLOAT3(0.5f, -0.5f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f,
		1.0f));
	pVertices[2] = CDiffusedVertex(XMFLOAT3(-0.5f, -0.5f, 0.0f), XMFLOAT4(Colors::Blue));
	//�ﰢ�� �޽��� ���ҽ�(���� ����)�� �����Ѵ�. 
	m_pd3dPositionBuffer = Util::CreateBufferResource(_Device, _CommandList, pVertices, m_nStride* m_nVertices, D3D12_HEAP_TYPE_DEFAULT,
		D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &m_pd3dPositionUploadBuffer);
		//���� ���� �並 �����Ѵ�. 

	D3D12_VERTEX_BUFFER_VIEW m_pd3dPositionBufferView;
	m_pd3dPositionBufferView.BufferLocation = m_pd3dPositionBuffer->GetGPUVirtualAddress();
	m_pd3dPositionBufferView.StrideInBytes = m_nStride;
		m_pd3dPositionBufferView.SizeInBytes = m_nStride * m_nVertices;

	m_pd3dVertexBufferViews.emplace_back(m_pd3dPositionBufferView);
}
